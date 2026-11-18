/*
  SIMBIO - Tramo 8b: bomba de aire y calentador CON AGUA (110 V por la caja de paso)
  ---------------------------------------------------------------------------------
  Requisitos: tramos 4, 5 y 8a aprobados; caja de paso revisada; garrafón con ~8 L de
  AGUA; calentador sumergido (marca mínima de inmersión bajo el agua) y con su perilla
  en el máximo (32 °C) solo para esta prueba; DS18B20 en el agua, lejos del calentador.
  Librerías: OneWire, DallasTemperature.
  Comandos (monitor serie a 115200, "Nueva línea"):
    on      control ON-OFF con histéresis ±0,5 °C (setpoint por defecto 30 °C)
    esc     prueba ESCALÓN: calentador encendido de forma continua (para AyC:
            curva de reacción T vs tiempo). Solo la cortan los enclavamientos.
    x       todo apagado            sp <C>   cambia el setpoint (25 a 35)
    a       bomba de aire on/off (manual)
  Salida: una línea CSV cada 10 s -> copie el monitor a un .csv o use el registro del
  Arduino IDE. Columnas: t_s,modo,T_C,nivel_cm,calentador,aire,bloqueo
  Enclavamientos (iguales al firmware): sin lectura de T o de nivel, nivel < 19 cm,
  o T >= 38 °C -> calentador apagado.
*/
#include <OneWire.h>
#include <DallasTemperature.h>

// --- Ajustes tomados de los tramos anteriores ---
const bool  RELE_ACTIVO_ALTO = true;     // resultado del tramo 8a
const float DIST_SENSOR_FONDO_CM = 30.0; // resultado del tramo 5 (comando "c")

const int RELE_AIRE = 21, RELE_CAL = 22, PIN_DS = 4, TRIG = 25, ECHO = 34;
const int BUZZER = 15, LED_ROJO = 16, LED_AMARILLO = 17, LED_VERDE = 2;
const float HIST = 0.5, T_MAX = 38.0, NIVEL_MIN = 19.0;

OneWire bus(PIN_DS);
DallasTemperature ds(&bus);

enum Modo { APAGADO, ONOFF, ESCALON } modo = APAGADO;
float setpoint = 30.0;
bool calentador = false, aire = false;
unsigned long tCalOn = 0;     // desde cuándo está encendido el calentador
float tAlEncender = NAN;
bool avisoNoSube = false;

void rele(int pin, bool on) { digitalWrite(pin, (on == RELE_ACTIVO_ALTO) ? HIGH : LOW); }

float leerNivel() {
  float v[5]; int n = 0;
  for (int i = 0; i < 5; i++) {
    digitalWrite(TRIG, LOW); delayMicroseconds(4);
    digitalWrite(TRIG, HIGH); delayMicroseconds(10); digitalWrite(TRIG, LOW);
    unsigned long us = pulseIn(ECHO, HIGH, 30000);
    if (us) v[n++] = us * 0.0343 / 2.0;
    delay(40);
  }
  if (n < 3) return NAN;
  for (int i = 0; i < n - 1; i++) for (int j = i + 1; j < n; j++) if (v[j] < v[i]) { float t = v[i]; v[i] = v[j]; v[j] = t; }
  return DIST_SENSOR_FONDO_CM - v[n / 2];
}

void setup() {
  digitalWrite(RELE_AIRE, RELE_ACTIVO_ALTO ? LOW : HIGH);
  digitalWrite(RELE_CAL,  RELE_ACTIVO_ALTO ? LOW : HIGH);
  pinMode(RELE_AIRE, OUTPUT); pinMode(RELE_CAL, OUTPUT);
  pinMode(TRIG, OUTPUT); pinMode(ECHO, INPUT);
  pinMode(BUZZER, OUTPUT); pinMode(LED_ROJO, OUTPUT); pinMode(LED_AMARILLO, OUTPUT); pinMode(LED_VERDE, OUTPUT);
  Serial.begin(115200);
  delay(300);
  ds.begin(); ds.setResolution(12);
  Serial.println("\n=== SIMBIO T08b: aire y calentador con agua ===");
  Serial.println("Comandos: on | esc | x | sp <C> | a");
  Serial.println("t_s,modo,T_C,nivel_cm,calentador,aire,bloqueo");
}

void comandos() {
  if (!Serial.available()) return;
  String c = Serial.readStringUntil('\n');
  c.trim();
  if (c == "on") modo = ONOFF;
  else if (c == "esc") modo = ESCALON;
  else if (c == "x") { modo = APAGADO; aire = false; }
  else if (c == "a") aire = !aire;
  else if (c.startsWith("sp ")) { float s = c.substring(3).toFloat(); if (s >= 25 && s <= 35) setpoint = s; }
  Serial.printf("# modo=%s setpoint=%.1f aire=%s\n",
                modo == ONOFF ? "ON-OFF" : modo == ESCALON ? "ESCALON" : "apagado", setpoint, aire ? "ON" : "off");
}

void loop() {
  static unsigned long tUlt = 0;
  comandos();
  if (millis() - tUlt < 10000 && tUlt != 0) return;
  tUlt = millis();

  ds.requestTemperatures();
  float t = ds.getTempCByIndex(0);
  bool tOk = t != DEVICE_DISCONNECTED_C && t > -20 && t < 100;
  float nivel = leerNivel();
  bool nOk = !isnan(nivel);

  const char* bloqueo = "-";
  if (!tOk) bloqueo = "sin_T";
  else if (!nOk) bloqueo = "sin_nivel";
  else if (nivel < NIVEL_MIN) bloqueo = "nivel_bajo";
  else if (t >= T_MAX) bloqueo = "T_max";

  bool quiere = calentador;
  if (modo == APAGADO) quiere = false;
  else if (modo == ESCALON) quiere = true;
  else if (tOk) { if (t < setpoint - HIST) quiere = true; else if (t > setpoint + HIST) quiere = false; }
  if (strcmp(bloqueo, "-") != 0) quiere = false;

  if (quiere && !calentador) { tCalOn = millis(); tAlEncender = t; avisoNoSube = false; }
  calentador = quiere;
  rele(RELE_CAL, calentador);
  rele(RELE_AIRE, aire);

  // Aviso: 60 min encendido y la T no ha subido 0,3 °C -> ¿calentador sin corriente o fuera del agua?
  if (calentador && !avisoNoSube && millis() - tCalOn > 3600000UL && tOk && t - tAlEncender < 0.3) {
    avisoNoSube = true;
    Serial.println("# AVISO: 60 min de calentador y la T no sube. Revise la caja de paso y el calentador.");
    digitalWrite(BUZZER, HIGH); delay(500); digitalWrite(BUZZER, LOW);
  }

  bool critico = strcmp(bloqueo, "-") != 0 && modo != APAGADO;
  digitalWrite(LED_ROJO, critico);
  digitalWrite(LED_AMARILLO, calentador);
  digitalWrite(LED_VERDE, aire);
  if (critico) { digitalWrite(BUZZER, HIGH); delay(100); digitalWrite(BUZZER, LOW); }

  Serial.printf("%lu,%s,%.2f,%.1f,%d,%d,%s\n", millis() / 1000,
                modo == ONOFF ? "onoff" : modo == ESCALON ? "escalon" : "apagado",
                t, nivel, calentador, aire, bloqueo);
}
