// =====================================================================
//  SIMBIO – Control del proceso y alarmas
//   * Temperatura: on/off con histéresis ±0,5 °C + enclavamientos
//   * Aireación: ciclos ON/OFF con minutos acumulados
//   * Melaza: dosis programadas con el servo (válvula de pinza)
// =====================================================================
#include "simbio.h"

static uint32_t tControl = 0;
static uint32_t ultimoMs = 0;
static uint32_t aireMsAcum = 0;          // fracción de segundo de aireación
static bool aireFaseOn = true;
static uint32_t aireFaseInicioS = 0;
static bool dosificando = false;
static uint32_t dosisFinMs = 0;

struct NombreAlarma { uint32_t bit; const char *nombre; };
static const NombreAlarma NOMBRES[] = {
  {AL_NIVEL_BAJO, "nivel_bajo"},     {AL_NIVEL_ALTO, "nivel_alto"},
  {AL_SENSOR_NIVEL, "sensor_nivel"}, {AL_TEMP_ALTA, "temp_alta"},
  {AL_SENSOR_TEMP, "sensor_temp"},   {AL_PH_BAJO, "ph_bajo"},
  {AL_TEMP_FUERA, "temp_fuera"},     {AL_PH_ALTO, "ph_alto"},
  {AL_SENSOR_PH, "sensor_ph"},       {AL_GAS, "gas_alto"},
  {AL_DHT, "sensor_dht"},            {AL_SD, "sd"},
  {AL_SIN_RED, "sin_red"},           {AL_RFID, "rfid"},
  {AL_CAL, "ph_sin_calibrar"},
};

const char *nombreAlarma(uint32_t bit) {
  for (auto &n : NOMBRES) if (n.bit == bit) return n.nombre;
  return "?";
}

String textoAlarmas(uint32_t m) {
  if (!m) return "-";
  String s;
  for (auto &n : NOMBRES) {
    if (m & n.bit) {
      if (s.length()) s += ",";
      s += n.nombre;
    }
  }
  return s;
}

const char *textoSemaforo() {
  if (E.modo != Modo::OPERACION) return "inactivo";
  if (E.alarmas & AL_CRITICAS) return "rojo";
  if (E.alarmas) return "amarillo";
  return "verde";
}

void controlInicio() {
  ultimoMs = millis();
  aireFaseOn = true;
  aireFaseInicioS = loteSegundos();
  dosificando = false;
}

void controlForzarDosis() {
  if (!E.lote.activo) {
    Serial.println("[CONTROL] No hay lote activo");
    return;
  }
  E.lote.proxDosisS = loteSegundos();
  Serial.println("[CONTROL] Dosis manual solicitada");
}

static void evaluarAlarmas() {
  uint32_t a = 0;
  uint32_t ls = loteSegundos();
  const Lecturas &L = E.lec;

  if (!L.okNivel) a |= AL_SENSOR_NIVEL;
  else {
    if (L.nivelCm < NIVEL_MIN_CM) a |= AL_NIVEL_BAJO;
    if (L.nivelCm > NIVEL_MAX_CM) a |= AL_NIVEL_ALTO;
  }
  if (!L.okTemp) a |= AL_SENSOR_TEMP;
  else {
    if (L.tempLiq >= T_MAX_C) a |= AL_TEMP_ALTA;
    else if (fabsf(L.tempLiq - E.setpoint) > T_TOLERANCIA_C && ls > T_GRACIA_S) a |= AL_TEMP_FUERA;
  }
  if (!L.okPh) a |= AL_SENSOR_PH;
  else {
    if (L.ph < PH_MIN_CRITICO) a |= AL_PH_BAJO;
    if (L.ph > PH_ALERTA_ALTO && ls > PH_GRACIA_S) a |= AL_PH_ALTO;
  }
  if (L.okGas && L.gasIdx > GAS_ALERTA_IDX) a |= AL_GAS;
  if (!L.okDht) a |= AL_DHT;
  if (!E.sdOk) a |= AL_SD;
  if (!E.wifiOk) a |= AL_SIN_RED;
  if (!E.rfidOk) a |= AL_RFID;
  if (!E.cal.valida) a |= AL_CAL;

  // Sin lote activo solo importan las fallas de equipo
  if (!E.lote.activo) a &= AL_INFRA;

  uint32_t nuevas = a & ~E.alarmas;
  uint32_t resueltas = E.alarmas & ~a;
  for (auto &n : NOMBRES) {
    if (nuevas & n.bit) {
      if (E.lote.activo) evento((n.bit & AL_CRITICAS) ? "alarma_critica" : "aviso", n.nombre);
      else Serial.printf("[ALARMA] %s\n", n.nombre);
    }
    if ((resueltas & n.bit) && E.lote.activo) evento("alarma_resuelta", n.nombre);
  }
  E.alarmas = a;
  E.silenciadas &= a;   // si una alarma silenciada desaparece y vuelve, suena otra vez
}

static void salidasSeguras() {
  E.sal.aire = false;
  E.sal.calentador = false;
  E.sal.valvula = false;
  dosificando = false;
}

static void controlTemperatura() {
  const Lecturas &L = E.lec;
  bool enclavado = !L.okTemp || !L.okNivel || L.nivelCm < NIVEL_MIN_CM || L.tempLiq >= T_MAX_C;
  if (enclavado) {
    E.sal.calentador = false;         // nunca calentar sin agua o sin lectura
    return;
  }
  if (L.tempLiq < E.setpoint - HISTERESIS_C) E.sal.calentador = true;
  else if (L.tempLiq > E.setpoint + HISTERESIS_C) E.sal.calentador = false;
}

static void controlAireacion(uint32_t ls, uint32_t dtMs) {
  uint32_t dur = aireFaseOn ? AIRE_ON_S : AIRE_OFF_S;
  if (ls - aireFaseInicioS >= dur) {
    aireFaseOn = !aireFaseOn;
    aireFaseInicioS = ls;
  }
  E.sal.aire = aireFaseOn;
  if (E.sal.aire) {
    aireMsAcum += dtMs;
    while (aireMsAcum >= 1000) { aireMsAcum -= 1000; E.lote.aireS++; }
  }
}

static void controlDosis(uint32_t ls) {
  if (dosificando) {
    if ((int32_t)(millis() - dosisFinMs) >= 0) {
      dosificando = false;
      E.sal.valvula = false;
      E.lote.dosis++;
      E.lote.proxDosisS = ls + DOSIS_INTERVALO_S;
      evento("dosis_melaza", String("dosis #") + E.lote.dosis + ", " + DOSIS_ABIERTA_MS + " ms");
      loteGuardar();
    }
    return;
  }
  if (ls < E.lote.proxDosisS) return;
  bool nivelOk = E.lec.okNivel && E.lec.nivelCm < NIVEL_MAX_CM - 1.0f;
  if (!nivelOk) {
    E.lote.proxDosisS = ls + DOSIS_REINTENTO_S;
    evento("dosis_pospuesta", "nivel alto o sin lectura de nivel");
    return;
  }
  dosificando = true;
  dosisFinMs = millis() + DOSIS_ABIERTA_MS;
  E.sal.valvula = true;
  Serial.println("[CONTROL] Abriendo válvula de melaza");
}

void controlActualizar() {
  uint32_t ahora = millis();
  if (ahora - tControl < 250) return;
  tControl = ahora;
  uint32_t dt = ahora - ultimoMs;
  ultimoMs = ahora;

  // Primeros 4 s: los sensores aún no tienen lectura; no evaluar para evitar falsas alarmas
  if (ahora < 4000) {
    salidasSeguras();
    return;
  }
  evaluarAlarmas();

  if (E.modo == Modo::CALIBRACION || !E.lote.activo) {
    salidasSeguras();
    return;
  }
  uint32_t ls = loteSegundos();
  controlTemperatura();
  controlAireacion(ls, dt);
  controlDosis(ls);
}
