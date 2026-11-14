#pragma once
// =====================================================================
//  SIMBIO – Tipos y estado compartido entre módulos
// =====================================================================
#include <Arduino.h>
#include "config.h"
#include "secrets.h"

// ---------------- Alarmas (máscara de bits) ----------------
enum : uint32_t {
  AL_NIVEL_BAJO   = 1u << 0,   // crítica: calentador descubierto
  AL_NIVEL_ALTO   = 1u << 1,   // crítica: riesgo de rebose
  AL_SENSOR_NIVEL = 1u << 2,   // crítica: sin lectura de nivel
  AL_TEMP_ALTA    = 1u << 3,   // crítica: sobretemperatura
  AL_SENSOR_TEMP  = 1u << 4,   // crítica: DS18B20 sin respuesta
  AL_PH_BAJO      = 1u << 5,   // crítica: pH demasiado ácido
  AL_TEMP_FUERA   = 1u << 6,   // aviso: fuera de setpoint ± tolerancia
  AL_PH_ALTO      = 1u << 7,   // aviso: no acidifica (posible contaminación)
  AL_SENSOR_PH    = 1u << 8,   // aviso: sonda pH desconectada/saturada
  AL_GAS          = 1u << 9,   // aviso: gases altos (MQ-135)
  AL_DHT          = 1u << 10,  // aviso: DHT sin respuesta
  AL_SD           = 1u << 11,  // aviso: SD no disponible
  AL_SIN_RED      = 1u << 12,  // aviso: sin WiFi
  AL_RFID         = 1u << 13,  // aviso: lector RFID sin respuesta
  AL_CAL          = 1u << 14,  // aviso: pH sin calibrar
};
constexpr uint32_t AL_CRITICAS = AL_NIVEL_BAJO | AL_NIVEL_ALTO | AL_SENSOR_NIVEL |
                                 AL_TEMP_ALTA | AL_SENSOR_TEMP | AL_PH_BAJO;
constexpr uint32_t AL_INFRA    = AL_SENSOR_NIVEL | AL_SENSOR_TEMP | AL_SENSOR_PH |
                                 AL_DHT | AL_SD | AL_SIN_RED | AL_RFID | AL_CAL;

enum class Modo : uint8_t { ESPERA, ARMADO, OPERACION, CALIBRACION };

struct Lecturas {
  float ph = NAN, phMv = NAN;         // phMv: voltaje del módulo (antes del divisor)
  float tempLiq = NAN;
  float tempAmb = NAN, humAmb = NAN;
  float distCm = NAN, nivelCm = NAN, volumenL = NAN;
  float gasMv = NAN, gasIdx = NAN;
  bool okPh = false, okTemp = false, okDht = false, okNivel = false, okGas = false;
};

struct Salidas {
  bool aire = false;
  bool calentador = false;
  bool valvula = false;
};

struct Lote {
  bool activo = false;
  char id[28] = "";
  char uid[24] = "";
  uint32_t inicioEpoch = 0;
  bool inicioEst = false;
  uint32_t segPrevios = 0;   // segundos acumulados antes de este arranque
  uint32_t aireS = 0;        // segundos de aireación acumulados
  uint16_t dosis = 0;
  uint32_t proxDosisS = 0;   // segundo del lote para la próxima dosis
};

struct Calibracion {
  float v7 = PH_V7_DEF_MV, v4 = PH_V4_DEF_MV, tCal = 25.0f;
  bool valida = CAL_DEFECTO_VALIDA;
};

struct Estado {
  Modo modo = Modo::ESPERA;
  Lecturas lec;
  Salidas sal;
  Lote lote;
  Calibracion cal;
  float setpoint = SETPOINT_DEF_C;
  uint32_t alarmas = 0;
  uint32_t silenciadas = 0;
  char uidArmado[24] = "";
  uint32_t armadoHastaMs = 0;
  volatile bool sdOk = false, rfidOk = false, wifiOk = false, horaNtp = false;
  uint32_t boots = 0;
};

extern Estado E;

// ---------------- Exclusión mutua del bus SPI (RFID + SD + cola) ----------------
extern SemaphoreHandle_t spiMutex;
struct SpiLock {
  SpiLock()  { xSemaphoreTake(spiMutex, portMAX_DELAY); }
  ~SpiLock() { xSemaphoreGive(spiMutex); }
};

// ---------------- sensores.cpp ----------------
void sensoresInicio();
void sensoresActualizar();

// ---------------- actuadores.cpp ----------------
void actuadoresInicio();
void actuadoresActualizar();
void buzzerPitido(uint16_t ms);

// ---------------- control.cpp ----------------
void controlInicio();
void controlActualizar();
void controlForzarDosis();
const char* nombreAlarma(uint32_t bit);
String textoAlarmas(uint32_t mascara);
const char* textoSemaforo();

// ---------------- lote.cpp ----------------
void loteInicio();
void loteActualizar();
bool loteIniciar(const char* uid);
void loteCerrar(const char* motivo);
uint32_t loteSegundos();
void loteGuardar();
void calRestablecer();
void setpointGuardar(float sp);

// ---------------- registro.cpp ----------------
void tiempoInicio();
void tiempoActualizar();
uint32_t epochAhora(bool &estimada);
void fmtIsoUtc(uint32_t epoch, char *buf, size_t n);
void fmtLocal(uint32_t epoch, char *buf, size_t n, const char *formato);
void registroInicio();
void registroActualizar();
void registroAbrirLote();
void registrarAhora();
void evento(const char *tipo, const String &detalle);
void encolarLote(bool cerrado);
uint32_t colaPendientes();
bool colaPeek(char &tipo, String &cuerpo, size_t maxLineas);
void colaConsumir();
void sdListar();
void sdVolcarCsvLote();

// ---------------- nube.cpp ----------------
void nubeInicio();
bool nubeConfigurada();

// ---------------- main.cpp ----------------
void imprimirEstado();
