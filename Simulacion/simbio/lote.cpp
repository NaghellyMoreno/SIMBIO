// =====================================================================
//  SIMBIO – Lote: RFID, pulsadores, DIP de calibración y persistencia
//
//  Flujo de operación:
//   1. En ESPERA, acerque una tarjeta/llavero al RC522 -> modo ARMADO (amarillo rápido)
//   2. Presione LOTE dentro de 15 s -> inicia el lote (verde)
//   3. LOTE sostenido 2,5 s -> cierra el lote
//   4. OK -> silencia la alarma sonora
//   5. DIP en ON -> modo CALIBRACIÓN del pH (buffer 7 con OK, luego buffer 4 con OK)
// =====================================================================
#include "simbio.h"
#include <SPI.h>
#include <MFRC522.h>
#include <Preferences.h>

static MFRC522 rfid(PIN_RFID_SS, PIN_RFID_RST);
static Preferences prefLote, prefCfg;

static uint32_t inicioMillisLote = 0;
static uint32_t tRfid = 0, tRfidSalud = 0, tGuardado = 0;
static char ultimoUid[24] = "";
static uint32_t ultimoUidMs = 0;
static uint8_t calPaso = 0;
static float calV7Tmp = 0;

// ---------------- Pulsadores con antirrebote y pulsación larga ----------------
struct Boton {
  uint8_t pin;
  bool estable = HIGH, lecturaPrevia = HIGH;
  uint32_t tCambio = 0, tPresion = 0;
  bool largoDisparado = false;
  explicit Boton(uint8_t p) : pin(p) {}
};
static Boton bLote(PIN_BTN_LOTE), bOk(PIN_BTN_OK);

static void leerBoton(Boton &b, bool &corto, bool &largo) {
  corto = largo = false;
  bool l = digitalRead(b.pin);
  uint32_t t = millis();
  if (l != b.lecturaPrevia) { b.lecturaPrevia = l; b.tCambio = t; }
  if (t - b.tCambio > 30 && l != b.estable) {
    b.estable = l;
    if (l == LOW) { b.tPresion = t; b.largoDisparado = false; }
    else if (!b.largoDisparado) corto = true;
  }
  if (b.estable == LOW && !b.largoDisparado && t - b.tPresion >= BOTON_LARGO_MS) {
    b.largoDisparado = true;
    largo = true;
  }
}

// ---------------- Persistencia ----------------
uint32_t loteSegundos() {
  if (!E.lote.activo) return E.lote.segPrevios;
  return E.lote.segPrevios + (millis() - inicioMillisLote) / 1000;
}

void loteGuardar() {
  prefLote.putBool("activo", E.lote.activo);
  prefLote.putString("id", E.lote.id);
  prefLote.putString("uid", E.lote.uid);
  prefLote.putULong("ini", E.lote.inicioEpoch);
  prefLote.putBool("iniEst", E.lote.inicioEst);
  prefLote.putULong("seg", loteSegundos());
  prefLote.putULong("aire", E.lote.aireS);
  prefLote.putUShort("dosis", E.lote.dosis);
  prefLote.putULong("prox", E.lote.proxDosisS);
}

static void loteCargar() {
  E.lote.activo = prefLote.getBool("activo", false);
  if (!E.lote.activo) return;
  prefLote.getString("id", E.lote.id, sizeof(E.lote.id));
  prefLote.getString("uid", E.lote.uid, sizeof(E.lote.uid));
  E.lote.inicioEpoch = prefLote.getULong("ini", 0);
  E.lote.inicioEst = prefLote.getBool("iniEst", true);
  E.lote.segPrevios = prefLote.getULong("seg", 0);
  E.lote.aireS = prefLote.getULong("aire", 0);
  E.lote.dosis = prefLote.getUShort("dosis", 0);
  E.lote.proxDosisS = prefLote.getULong("prox", DOSIS_INTERVALO_S);
  inicioMillisLote = millis();
}

static void calCargar() {
  E.cal.v7 = prefCfg.getFloat("v7", PH_V7_DEF_MV);
  E.cal.v4 = prefCfg.getFloat("v4", PH_V4_DEF_MV);
  E.cal.tCal = prefCfg.getFloat("tc", 25.0f);
  E.cal.valida = prefCfg.getBool("calOk", CAL_DEFECTO_VALIDA);
  E.setpoint = prefCfg.getFloat("sp", SETPOINT_DEF_C);
}

void calRestablecer() {
  prefCfg.remove("v7"); prefCfg.remove("v4"); prefCfg.remove("tc"); prefCfg.remove("calOk");
  calCargar();
  Serial.println("[CAL] Calibración restablecida a valores por defecto");
}

void setpointGuardar(float sp) {
  E.setpoint = constrain(sp, 15.0f, 37.0f);
  prefCfg.putFloat("sp", E.setpoint);
  Serial.printf("[CONTROL] Setpoint = %.1f °C\n", E.setpoint);
  if (E.lote.activo) evento("setpoint", String(E.setpoint, 1));
}

// ---------------- Inicio / cierre de lote ----------------
static void uidATexto(const MFRC522::Uid &u, char *buf, size_t n) {
  size_t p = 0;
  buf[0] = 0;
  for (byte i = 0; i < u.size && p + 3 < n; i++) {
    p += snprintf(buf + p, n - p, i ? ":%02X" : "%02X", u.uidByte[i]);
  }
}

bool loteIniciar(const char *uid) {
  if (E.lote.activo) return false;
  bool est;
  uint32_t ep = epochAhora(est);
  Lote nuevo;
  nuevo.activo = true;
  strlcpy(nuevo.uid, uid, sizeof(nuevo.uid));
  if (ep > 0) {
    char f[20];
    fmtLocal(ep, f, sizeof(f), "%Y%m%d-%H%M%S");
    snprintf(nuevo.id, sizeof(nuevo.id), "MM-%s", f);
  } else {
    // sin ninguna referencia de hora: UID + número de arranque
    char corto[8] = "";
    size_t L = strlen(uid);
    size_t j = 0;
    for (size_t i = (L > 8 ? L - 8 : 0); i < L && j < 6; i++) if (uid[i] != ':') corto[j++] = uid[i];
    corto[j] = 0;
    snprintf(nuevo.id, sizeof(nuevo.id), "MM-%s-%lu", corto, (unsigned long)E.boots);
  }
  nuevo.inicioEpoch = ep;
  nuevo.inicioEst = est;
  nuevo.proxDosisS = DOSIS_INTERVALO_S;
  E.lote = nuevo;
  inicioMillisLote = millis();
  E.modo = Modo::OPERACION;
  E.silenciadas = 0;
  controlInicio();
  loteGuardar();
  registroAbrirLote();
  encolarLote(false);
  evento("inicio_lote", String("tarjeta ") + uid);
  registrarAhora();
  buzzerPitido(300);
  Serial.printf("\n*** LOTE %s INICIADO (tarjeta %s) ***\n\n", E.lote.id, uid);
  return true;
}

void loteCerrar(const char *motivo) {
  if (!E.lote.activo) {
    Serial.println("[LOTE] No hay lote activo");
    return;
  }
  registrarAhora();                       // última lectura
  evento("cierre_lote", motivo);
  E.lote.segPrevios = loteSegundos();
  E.lote.activo = false;
  encolarLote(true);
  loteGuardar();
  E.modo = Modo::ESPERA;
  E.sal = Salidas();
  buzzerPitido(800);
  Serial.printf("\n*** LOTE %s CERRADO: %lu s, aire %.1f min, %u dosis ***\n\n", E.lote.id,
                (unsigned long)E.lote.segPrevios, E.lote.aireS / 60.0f, E.lote.dosis);
}

// ---------------- RFID ----------------
static void rfidIniciar() {
  SpiLock l;
  rfid.PCD_Init();
  byte v = rfid.PCD_ReadRegister(MFRC522::VersionReg);
  E.rfidOk = (v != 0x00 && v != 0xFF);
  Serial.printf("[RFID] RC522 versión 0x%02X %s\n", v, E.rfidOk ? "OK" : "NO RESPONDE");
}

static void tarjetaLeida(const char *uid) {
  Serial.printf("[RFID] Tarjeta %s\n", uid);
  switch (E.modo) {
    case Modo::ESPERA:
    case Modo::ARMADO:
      strlcpy(E.uidArmado, uid, sizeof(E.uidArmado));
      E.armadoHastaMs = millis() + ARMADO_TIMEOUT_MS;
      E.modo = Modo::ARMADO;
      buzzerPitido(120);
      Serial.println(">> Presione LOTE en los próximos 15 s para iniciar un lote con esta tarjeta");
      break;
    case Modo::OPERACION:
      buzzerPitido(120);
      if (strcmp(uid, E.lote.uid) == 0) {
        imprimirEstado();
      } else {
        Serial.printf(">> Hay un lote activo (%s, tarjeta %s). Ciérrelo antes de iniciar otro.\n",
                      E.lote.id, E.lote.uid);
        evento("tarjeta_ajena", uid);
      }
      break;
    case Modo::CALIBRACION:
      Serial.println(">> En calibración no se leen lotes");
      break;
  }
}

static void sondearRfid() {
  char uid[24] = "";
  {
    SpiLock l;
    if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
      uidATexto(rfid.uid, uid, sizeof(uid));
      rfid.PICC_HaltA();
    }
  }
  if (!uid[0]) return;
  if (strcmp(uid, ultimoUid) == 0 && millis() - ultimoUidMs < 2000) return;   // rebote
  strlcpy(ultimoUid, uid, sizeof(ultimoUid));
  ultimoUidMs = millis();
  tarjetaLeida(uid);
}

// ---------------- Calibración del pH ----------------
static void entrarCalibracion() {
  E.modo = Modo::CALIBRACION;
  calPaso = 0;
  Serial.println("\n=== MODO CALIBRACIÓN DE pH (DIP en ON) ===");
  Serial.println("Salidas apagadas por seguridad.");
  Serial.println("1) Sumerja la sonda en buffer pH 7, espere a que se estabilice y presione OK.");
#if SIMBIO_SIM
  Serial.println("   (Wokwi: ponga el deslizador de pH en el valor que quiera tomar como pH 7)");
#endif
}

static void salirCalibracion() {
  E.modo = E.lote.activo ? Modo::OPERACION : Modo::ESPERA;
  Serial.println("=== FIN DE CALIBRACIÓN ===\n");
  if (E.lote.activo) controlInicio();
}

static void pasoCalibracion() {
  if (isnan(E.lec.phMv)) {
    Serial.println("[CAL] Sin lectura de la sonda, revise la conexión");
    return;
  }
  if (calPaso == 0) {
    calV7Tmp = E.lec.phMv;
    calPaso = 1;
    buzzerPitido(150);
    Serial.printf("[CAL] pH 7 = %.0f mV. 2) Enjuague, sumerja en buffer pH 4 y presione OK.\n", calV7Tmp);
    return;
  }
  float v4 = E.lec.phMv;
  float pend = (v4 - calV7Tmp) / (4.0f - 7.0f);
  if (v4 <= calV7Tmp || fabsf(pend) < PH_PEND_MIN || fabsf(pend) > PH_PEND_MAX) {
    Serial.printf("[CAL] Rechazada: pendiente %.1f mV/pH (esperado %.0f a %.0f). Repita desde pH 7.\n",
                  fabsf(pend), PH_PEND_MIN, PH_PEND_MAX);
    calPaso = 0;
    buzzerPitido(600);
    return;
  }
  E.cal.v7 = calV7Tmp;
  E.cal.v4 = v4;
  E.cal.tCal = E.lec.okTemp ? E.lec.tempLiq : 25.0f;
  E.cal.valida = true;
  prefCfg.putFloat("v7", E.cal.v7);
  prefCfg.putFloat("v4", E.cal.v4);
  prefCfg.putFloat("tc", E.cal.tCal);
  prefCfg.putBool("calOk", true);
  calPaso = 0;
  buzzerPitido(300);
  String det = String("v7=") + String(E.cal.v7, 0) + " mV, v4=" + String(E.cal.v4, 0) +
               " mV, pendiente=" + String(fabsf(pend), 1) + " mV/pH, T=" + String(E.cal.tCal, 1) + " C";
  Serial.println("[CAL] Calibración guardada: " + det);
  evento("calibracion_ph", det);
  Serial.println("Ponga el DIP en OFF para volver a operación (o presione OK para repetir).");
}

// ---------------- API ----------------
void loteInicio() {
  pinMode(PIN_BTN_LOTE, INPUT_PULLUP);
  pinMode(PIN_BTN_OK, INPUT_PULLUP);
  pinMode(PIN_DIP_CAL, INPUT);           // GPIO 35 no tiene pull-up interno: 10 k externo
  prefLote.begin("lote", false);
  prefCfg.begin("cfg", false);
  calCargar();
  loteCargar();
  rfidIniciar();
  if (E.lote.activo) {
    E.modo = Modo::OPERACION;
    controlInicio();
    Serial.printf("[LOTE] Reanudando lote %s (%lu s transcurridos)\n", E.lote.id,
                  (unsigned long)E.lote.segPrevios);
    registroAbrirLote();
    evento("reanudacion", "reinicio del equipo; el tiempo sin energía no se cuenta");
  }
  if (digitalRead(PIN_DIP_CAL) == LOW) entrarCalibracion();
}

void loteActualizar() {
  uint32_t ahora = millis();

  // DIP de calibración
  bool dipCal = digitalRead(PIN_DIP_CAL) == LOW;
  if (dipCal && E.modo != Modo::CALIBRACION) entrarCalibracion();
  else if (!dipCal && E.modo == Modo::CALIBRACION) salirCalibracion();

  // Pulsadores
  bool loteCorto, loteLargo, okCorto, okLargo;
  leerBoton(bLote, loteCorto, loteLargo);
  leerBoton(bOk, okCorto, okLargo);

  switch (E.modo) {
    case Modo::CALIBRACION:
      if (okCorto) pasoCalibracion();
      if (loteCorto) { calPaso = 0; Serial.println("[CAL] Reiniciada. Buffer pH 7 y OK."); }
      break;
    case Modo::ARMADO:
      if (loteCorto) loteIniciar(E.uidArmado);
      else if ((int32_t)(ahora - E.armadoHastaMs) > 0) {
        E.modo = Modo::ESPERA;
        Serial.println("[LOTE] Tiempo agotado, no se inició el lote");
      }
      break;
    case Modo::ESPERA:
      if (loteCorto) Serial.println(">> Acerque la tarjeta del lote al lector y luego presione LOTE");
      break;
    case Modo::OPERACION:
      if (loteLargo) loteCerrar("pulsador LOTE");
      else if (loteCorto) imprimirEstado();
      if (okCorto) {
        E.silenciadas = E.alarmas & AL_CRITICAS;
        Serial.println("[ALARMA] Alarma sonora silenciada");
      }
      break;
  }

  // RFID cada 150 ms y chequeo de salud cada 10 s
  if (ahora - tRfid >= 150) { tRfid = ahora; sondearRfid(); }
  if (ahora - tRfidSalud >= 10000) {
    tRfidSalud = ahora;
    byte v;
    { SpiLock l; v = rfid.PCD_ReadRegister(MFRC522::VersionReg); }
    bool ok = (v != 0x00 && v != 0xFF);
    if (!ok) rfidIniciar();
    else E.rfidOk = true;
  }

  // Guardado periódico del lote (para reanudar tras un corte)
  if (E.lote.activo && ahora - tGuardado >= (uint32_t)LOTE_GUARDAR_S * 1000UL) {
    tGuardado = ahora;
    loteGuardar();
  }
}
