// =====================================================================
//  SIMBIO – Sensores: pH, DS18B20, DHT, HC-SR04, MQ-135
//  Lecturas no bloqueantes con un temporizador por sensor.
// =====================================================================
#include "simbio.h"
#include <utility>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <DHTesp.h>

static OneWire oneWire(PIN_DS18B20);
static DallasTemperature ds18b20(&oneWire);
static DHTesp dht;

static uint32_t tAnalog = 0, tDs = 0, tDsPedido = 0, tDht = 0, tNivel = 0;
static bool dsPedido = false;
static uint8_t fallosDs = 0, fallosDht = 0, fallosNivel = 0;
static uint32_t arranqueMs = 0;

// Promedio de N lecturas en mV en el pin del ESP32 (después del divisor)
static float leerPinMv(uint8_t pin, int n) {
  uint32_t acc = 0;
  for (int i = 0; i < n; i++) {
#if SIMBIO_SIM
    // Wokwi: ADC ideal y lineal 0–3,3 V
    acc += (uint32_t)analogRead(pin) * 3300UL / 4095UL;
#else
    // ESP32 real: usa la calibración de fábrica (eFuse) del ADC
    acc += analogReadMilliVolts(pin);
#endif
  }
  return (float)acc / n;
}

// pH con calibración de 2 puntos y compensación de temperatura (Nernst)
static float calcularPh(float mvModulo, float tempC) {
  float pend = (E.cal.v4 - E.cal.v7) / (4.0f - 7.0f);   // mV/pH (negativa)
  float factor = (tempC + 273.15f) / (E.cal.tCal + 273.15f);
  return 7.0f + (mvModulo - E.cal.v7) / (pend * factor);
}

void sensoresInicio() {
  arranqueMs = millis();
  analogReadResolution(12);
  analogSetPinAttenuation(PIN_PH, ADC_11db);
  analogSetPinAttenuation(PIN_MQ135, ADC_11db);

  pinMode(PIN_US_TRIG, OUTPUT);
  digitalWrite(PIN_US_TRIG, LOW);
  pinMode(PIN_US_ECHO, INPUT);

  ds18b20.begin();
  ds18b20.setWaitForConversion(false);
  ds18b20.setResolution(12);
  Serial.printf("[SENSORES] DS18B20 encontrados: %d\n", ds18b20.getDeviceCount());

#if DHT_ES_DHT22
  dht.setup(PIN_DHT, DHTesp::DHT22);
#else
  dht.setup(PIN_DHT, DHTesp::DHT11);
#endif
}

static void leerAnalogicos() {
  // --- pH ---
  float pinMv = leerPinMv(PIN_PH, ADC_PROMEDIO);
  bool phValido = pinMv > 50.0f && pinMv < 3250.0f;       // 0 V o saturado = sonda desconectada
  float mvModulo = pinMv / DIVISOR_ANALOG;
  if (phValido) {
    E.lec.phMv = isnan(E.lec.phMv) ? mvModulo : (0.7f * E.lec.phMv + 0.3f * mvModulo);  // filtro
    float t = E.lec.okTemp ? E.lec.tempLiq : 25.0f;
    float ph = calcularPh(E.lec.phMv, t);
    phValido = ph > -0.5f && ph < 14.5f;
    E.lec.ph = constrain(ph, 0.0f, 14.0f);
  }
  E.lec.okPh = phValido;

  // --- MQ-135: índice cualitativo 0–100 (voltaje del módulo / 50 mV) ---
  float gasPin = leerPinMv(PIN_MQ135, 32);
  E.lec.gasMv = gasPin / DIVISOR_ANALOG;
  E.lec.gasIdx = constrain(E.lec.gasMv / 50.0f, 0.0f, 100.0f);
  E.lec.okGas = (millis() - arranqueMs) > (uint32_t)GAS_PRECAL_S * 1000UL;   // precalentamiento
}

static void leerDs18b20() {
  if (!dsPedido) {
    if (ds18b20.getDeviceCount() == 0) ds18b20.begin();
    ds18b20.requestTemperatures();
    dsPedido = true;
    tDsPedido = millis();
    return;
  }
  if (millis() - tDsPedido < 800) return;   // conversión de 12 bits: 750 ms
  dsPedido = false;
  float t = ds18b20.getTempCByIndex(0);
  if (t == DEVICE_DISCONNECTED_C || t < -20.0f || t > 100.0f) {
    if (fallosDs < 255) fallosDs++;
  } else {
    fallosDs = 0;
    E.lec.tempLiq = t;
  }
  E.lec.okTemp = fallosDs < 3 && !isnan(E.lec.tempLiq);
}

static void leerDht() {
  TempAndHumidity th = dht.getTempAndHumidity();
  if (dht.getStatus() == DHTesp::ERROR_NONE && !isnan(th.temperature)) {
    fallosDht = 0;
    E.lec.tempAmb = th.temperature;
    E.lec.humAmb = th.humidity;
  } else if (fallosDht < 255) {
    fallosDht++;
  }
  E.lec.okDht = fallosDht < 3 && !isnan(E.lec.tempAmb);
}

static float pingCm() {
  digitalWrite(PIN_US_TRIG, LOW);
  delayMicroseconds(3);
  digitalWrite(PIN_US_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_US_TRIG, LOW);
  unsigned long dur = pulseIn(PIN_US_ECHO, HIGH, 30000UL);
  if (dur == 0) return NAN;
  return dur * 0.0343f / 2.0f;
}

static void leerNivel() {
  float m[3];
  int n = 0;
  for (int i = 0; i < 3; i++) {
    float d = pingCm();
    if (!isnan(d) && d >= 2.0f && d <= 400.0f) m[n++] = d;
    delay(15);
  }
  if (n == 0) {
    if (fallosNivel < 255) fallosNivel++;
  } else {
    // mediana (o promedio si hay menos de 3)
    if (n == 3) {
      if (m[0] > m[1]) std::swap(m[0], m[1]);
      if (m[1] > m[2]) std::swap(m[1], m[2]);
      if (m[0] > m[1]) std::swap(m[0], m[1]);
      E.lec.distCm = m[1];
    } else {
      E.lec.distCm = (n == 2) ? (m[0] + m[1]) / 2.0f : m[0];
    }
    fallosNivel = 0;
    float nivel = DIST_SENSOR_FONDO_CM - E.lec.distCm;
    E.lec.nivelCm = nivel < 0 ? 0 : nivel;
    E.lec.volumenL = E.lec.nivelCm * AREA_TANQUE_CM2 / 1000.0f;
  }
  E.lec.okNivel = fallosNivel < 3 && !isnan(E.lec.nivelCm);
}

void sensoresActualizar() {
  uint32_t ahora = millis();
  if (ahora - tAnalog >= 500) { tAnalog = ahora; leerAnalogicos(); }
  if (ahora - tDs >= 250)     { tDs = ahora; leerDs18b20(); }
  if (ahora - tDht >= 3000)   { tDht = ahora; leerDht(); }
  if (ahora - tNivel >= 1000) { tNivel = ahora; leerNivel(); }
}
