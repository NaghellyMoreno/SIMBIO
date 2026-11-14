// =====================================================================
//  SIMBIO – Sistema IoT de monitoreo, control y trazabilidad por lote
//  para la producción artesanal de bioinsumos (activación líquida de MM)
//  Universidad de Caldas – Automatización y Control / Fundamentos de IoT
//
//  Entradas (6): pH, temperatura del líquido, nivel, gases, RFID del lote,
//                temperatura y humedad ambiente
//  Salidas  (4): bomba de aire, calentador, válvula de melaza (servo),
//                alarma (semáforo + buzzer)
//
//  Arduino IDE: placa "ESP32 Dev Module". Modo simulación/real en config.h.
//  Para Wokwi: Sketch -> Export Compiled Binary (Opción+Cmd+S).
//  Monitor serie a 115200. Escriba "ayuda" para ver los comandos.
// =====================================================================
#include "simbio.h"
#include <SPI.h>
#include <WiFi.h>

Estado E;
SemaphoreHandle_t spiMutex;

static const char *textoModo() {
  switch (E.modo) {
    case Modo::ESPERA: return "ESPERA";
    case Modo::ARMADO: return "ARMADO";
    case Modo::OPERACION: return "OPERACION";
    case Modo::CALIBRACION: return "CALIBRACION";
  }
  return "?";
}

static String fmtNum(float v, bool ok, int dec, const char *unidad) {
  if (!ok || isnan(v)) return "--";
  return String(v, dec) + unidad;
}

void imprimirEstado() {
  const Lecturas &L = E.lec;
  bool est;
  uint32_t ep = epochAhora(est);
  char hora[24] = "sin hora";
  if (ep > 0) fmtLocal(ep, hora, sizeof(hora), "%Y-%m-%d %H:%M:%S");
  Serial.println("------------------------------------------------------------------");
  Serial.printf(" %s%s | modo %s", hora, est && ep ? " (estimada)" : "", textoModo());
  if (E.lote.activo) Serial.printf(" | lote %s, %lu s", E.lote.id, (unsigned long)loteSegundos());
  Serial.println();
  Serial.printf(" pH %s (%s) | T liquido %s (SP %.1f) | T amb %s HR %s\n",
                fmtNum(L.ph, L.okPh, 2, "").c_str(), fmtNum(L.phMv, L.okPh, 0, " mV").c_str(),
                fmtNum(L.tempLiq, L.okTemp, 2, " C").c_str(), E.setpoint,
                fmtNum(L.tempAmb, L.okDht, 1, " C").c_str(), fmtNum(L.humAmb, L.okDht, 0, " %").c_str());
  Serial.printf(" Nivel %s (%s) | Gas %s\n",
                fmtNum(L.nivelCm, L.okNivel, 1, " cm").c_str(), fmtNum(L.volumenL, L.okNivel, 2, " L").c_str(),
                L.okGas ? fmtNum(L.gasIdx, true, 0, "/100").c_str() : "precalentando");
  Serial.printf(" Aire %s | Calentador %s | Valvula %s | Aire acum %.1f min | Dosis %u\n",
                E.sal.aire ? "ON" : "off", E.sal.calentador ? "ON" : "off",
                E.sal.valvula ? "ABIERTA" : "cerrada", E.lote.aireS / 60.0f, E.lote.dosis);
  Serial.printf(" Alarmas: %s%s\n", textoAlarmas(E.alarmas).c_str(),
                (E.silenciadas ? " (sonido silenciado)" : ""));
  Serial.printf(" WiFi %s | SD %s | RFID %s | Nube: %s, %lu pendientes\n",
                E.wifiOk ? "OK" : "no", E.sdOk ? "OK" : "no", E.rfidOk ? "OK" : "no",
                nubeConfigurada() ? "Supabase" : "sin configurar", (unsigned long)colaPendientes());
  Serial.println("------------------------------------------------------------------");
}

static void ayuda() {
  Serial.println(
    "\nComandos (Monitor serie):\n"
    "  estado        muestra lecturas y salidas\n"
    "  sp <C>        cambia el setpoint de temperatura (ej. sp 30)\n"
    "  dosis         fuerza una dosis de melaza\n"
    "  cerrar        cierra el lote activo\n"
    "  sd            lista los archivos de la SD\n"
    "  csv           muestra el CSV del lote actual\n"
    "  calreset      vuelve a la calibración de pH por defecto\n"
    "  wifi          estado de la red\n"
    "  reiniciar     reinicia el ESP32 (prueba de reanudación del lote)\n"
    "Operación: tarjeta RFID -> LOTE para iniciar | LOTE 2,5 s para cerrar | OK silencia\n"
    "           DIP 1 en ON = calibración de pH\n");
}

static void comandos() {
  static String linea;
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\r') continue;
    if (c != '\n') {
      if (linea.length() < 60) linea += c;
      continue;
    }
    linea.trim();
    String cmd = linea;
    linea = "";
    cmd.toLowerCase();
    if (cmd.isEmpty()) continue;
    if (cmd == "ayuda" || cmd == "help") ayuda();
    else if (cmd == "estado") imprimirEstado();
    else if (cmd.startsWith("sp ")) setpointGuardar(cmd.substring(3).toFloat());
    else if (cmd == "dosis") controlForzarDosis();
    else if (cmd == "cerrar") loteCerrar("comando serie");
    else if (cmd == "sd") sdListar();
    else if (cmd == "csv") sdVolcarCsvLote();
    else if (cmd == "calreset") calRestablecer();
    else if (cmd == "wifi") Serial.printf("[WIFI] %s, IP %s, RSSI %d\n", E.wifiOk ? "conectado" : "desconectado",
                                          WiFi.localIP().toString().c_str(), WiFi.RSSI());
    else if (cmd == "reiniciar") { if (E.lote.activo) loteGuardar(); delay(100); ESP.restart(); }
    else Serial.println("Comando no reconocido. Escriba \"ayuda\".");
  }
}

void setup() {
  Serial.begin(115200);
  delay(300);
  Serial.println("\n==================================================================");
  Serial.printf(" SIMBIO %s  –  %s  (%s)\n", VERSION_FW, DISPOSITIVO_ID,
                SIMBIO_SIM ? "SIMULACION Wokwi, tiempos acelerados" : "prototipo real");
  Serial.println(" Trazabilidad por lote – activación líquida de MM");
  Serial.println("==================================================================");

  spiMutex = xSemaphoreCreateMutex();

  // Bus SPI compartido: RC522 (CS 5) y SD (CS 26). Ambos CS en alto antes de iniciar.
  pinMode(PIN_RFID_SS, OUTPUT);
  digitalWrite(PIN_RFID_SS, HIGH);
  pinMode(PIN_SD_CS, OUTPUT);
  digitalWrite(PIN_SD_CS, HIGH);
  SPI.begin(PIN_SPI_SCK, PIN_SPI_MISO, PIN_SPI_MOSI);

  tiempoInicio();
  actuadoresInicio();
  sensoresInicio();
  registroInicio();
  loteInicio();
  nubeInicio();
  ayuda();
}

void loop() {
  sensoresActualizar();
  loteActualizar();
  controlActualizar();
  actuadoresActualizar();
  tiempoActualizar();
  registroActualizar();
  comandos();
  delay(5);
}
