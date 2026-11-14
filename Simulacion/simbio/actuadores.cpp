// =====================================================================
//  SIMBIO – Actuadores: relés, servo (válvula de melaza), semáforo, buzzer
// =====================================================================
#include "simbio.h"

// ---- Compatibilidad del PWM (LEDC) con el núcleo ESP32 2.x y 3.x ----
// El gestor de placas de Arduino IDE instala hoy el núcleo 3.x, que usa pines
// en lugar de canales. Este bloque permite compilar con cualquiera de los dos.
#if ESP_ARDUINO_VERSION_MAJOR >= 3
#define PWM_SERVO  PIN_SERVO
#define PWM_BUZZER PIN_BUZZER
static void pwmIniciar(uint8_t pin, uint8_t, uint32_t frec, uint8_t bits) { ledcAttach(pin, frec, bits); }
#else
#define PWM_SERVO  CANAL_LEDC_SERVO
#define PWM_BUZZER CANAL_LEDC_BUZZER
static void pwmIniciar(uint8_t pin, uint8_t canal, uint32_t frec, uint8_t bits) {
  ledcSetup(canal, frec, bits);
  ledcAttachPin(pin, canal);
}
#endif

static int anguloServo = -1;
static uint32_t pitidoHastaMs = 0;
static bool buzzerEncendido = false;

static void rele(uint8_t pin, bool encendido) {
  bool nivel = RELE_ACTIVO_ALTO ? encendido : !encendido;
  digitalWrite(pin, nivel ? HIGH : LOW);
}

// Servo por LEDC a 50 Hz (16 bits): sin librerías externas
static void servoAngulo(int grados) {
  if (grados == anguloServo) return;
  anguloServo = grados;
  uint32_t us = map(constrain(grados, 0, 180), 0, 180, 500, 2400);
  uint32_t duty = (uint32_t)((uint64_t)us * 65535ULL / 20000ULL);
  ledcWrite(PWM_SERVO, duty);
}

static void buzzer(bool on) {
  if (on == buzzerEncendido) return;
  buzzerEncendido = on;
#if BUZZER_ACTIVO
  digitalWrite(PIN_BUZZER, on ? HIGH : LOW);
#else
  ledcWriteTone(PWM_BUZZER, on ? 2200 : 0);
#endif
}

void buzzerPitido(uint16_t ms) { pitidoHastaMs = millis() + ms; }

void actuadoresInicio() {
  pinMode(PIN_RELE_AIRE, OUTPUT);
  pinMode(PIN_RELE_CALENTADOR, OUTPUT);
  rele(PIN_RELE_AIRE, false);
  rele(PIN_RELE_CALENTADOR, false);

  pinMode(PIN_LED_ROJO, OUTPUT);
  pinMode(PIN_LED_AMARILLO, OUTPUT);
  pinMode(PIN_LED_VERDE, OUTPUT);

  pwmIniciar(PIN_SERVO, CANAL_LEDC_SERVO, 50, 16);
  servoAngulo(SERVO_CERRADO_GRAD);

#if BUZZER_ACTIVO
  pinMode(PIN_BUZZER, OUTPUT);
  digitalWrite(PIN_BUZZER, LOW);
#else
  pwmIniciar(PIN_BUZZER, CANAL_LEDC_BUZZER, 2200, 10);
  ledcWrite(PWM_BUZZER, 0);
#endif
}

static void leds(bool r, bool a, bool v) {
  digitalWrite(PIN_LED_ROJO, r);
  digitalWrite(PIN_LED_AMARILLO, a);
  digitalWrite(PIN_LED_VERDE, v);
}

static void semaforo() {
  uint32_t t = millis();
  bool lento = (t % 2000) < 150;     // destello corto cada 2 s
  bool rapido = (t % 400) < 200;
  bool medio = (t % 1000) < 500;
  uint32_t avisos = E.alarmas & ~AL_CRITICAS;
  uint32_t criticas = E.alarmas & AL_CRITICAS;

  switch (E.modo) {
    case Modo::CALIBRACION:
      leds(false, true, medio);
      break;
    case Modo::ARMADO:
      leds(false, rapido, false);
      break;
    case Modo::ESPERA:
      if (criticas || avisos) leds(false, lento, false);
      else leds(false, false, lento);
      break;
    case Modo::OPERACION:
      if (criticas) leds(true, false, false);
      else if (avisos) leds(false, true, false);
      else leds(false, false, true);
      break;
  }
}

void actuadoresActualizar() {
  rele(PIN_RELE_AIRE, E.sal.aire);
  rele(PIN_RELE_CALENTADOR, E.sal.calentador);
  servoAngulo(E.sal.valvula ? SERVO_ABIERTO_GRAD : SERVO_CERRADO_GRAD);
  semaforo();

  // Buzzer: alarmas críticas no silenciadas (patrón 200 ms / 1 s) o pitido de confirmación
  bool alarmaSonora = E.modo == Modo::OPERACION &&
                      (E.alarmas & AL_CRITICAS & ~E.silenciadas) != 0 &&
                      (millis() % 1000) < 200;
  bool pitido = (int32_t)(pitidoHastaMs - millis()) > 0;
  buzzer(alarmaSonora || pitido);
}
