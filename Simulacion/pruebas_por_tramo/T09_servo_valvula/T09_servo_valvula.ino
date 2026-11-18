/*
  SIMBIO - Tramo 9 (con AGUA) y Tramo 11 (con MELAZA): válvula de pinza con servo SG90
  -------------------------------------------------------------------------------------
  Sin librerías: el servo se maneja con LEDC a 50 Hz, igual que el firmware principal
  (funciona con el núcleo ESP32 2.x y 3.x).
  Conexiones del SG90: café/negro GND, rojo 5 V del riel (NO del pin 3V3 del ESP32),
  naranja/amarillo (señal) GPIO 14. Capacitor de 470 uF entre 5 V y GND cerca del servo.
  Comandos (monitor serie a 115200, "Nueva línea"):
    g <grados>  mueve el servo a ese ángulo (para buscar CERRADO y ABIERTO)
    c / o       cierra / abre con los ángulos actuales
    sc <grados> fija el ángulo CERRADO    so <grados> fija el ángulo ABIERTO
    d <ms>      una dosis: abre, espera <ms>, cierra   (ej. d 8000)
    r <n>       repite n dosis con el último <ms>, 30 s entre dosis (repetibilidad)
  Al final copie los ángulos a config.h: SERVO_CERRADO_GRAD y SERVO_ABIERTO_GRAD,
  y el tiempo elegido a DOSIS_ABIERTA_MS.
*/
const int PIN_SERVO = 14;
int anguloCerrado = 0, anguloAbierto = 90;
unsigned long dosisMs = 8000;

#if ESP_ARDUINO_VERSION_MAJOR >= 3
void pwmIniciar() { ledcAttach(PIN_SERVO, 50, 16); }
void pwmEscribir(uint32_t duty) { ledcWrite(PIN_SERVO, duty); }
#else
const int CANAL = 2;
void pwmIniciar() { ledcSetup(CANAL, 50, 16); ledcAttachPin(PIN_SERVO, CANAL); }
void pwmEscribir(uint32_t duty) { ledcWrite(CANAL, duty); }
#endif

void servo(int grados) {
  grados = constrain(grados, 0, 180);
  uint32_t us = map(grados, 0, 180, 500, 2400);           // mismo rango que el firmware
  pwmEscribir((uint32_t)((uint64_t)us * 65535ULL / 20000ULL));
  Serial.printf("servo -> %d°\n", grados);
}

void dosis(int n) {
  Serial.printf("# dosis %d: abierta %lu ms\n", n, dosisMs);
  unsigned long t0 = millis();
  servo(anguloAbierto);
  delay(dosisMs);
  servo(anguloCerrado);
  Serial.printf("# dosis %d terminada (%lu ms reales). Pese o mida el volumen ahora.\n", n, millis() - t0);
}

void setup() {
  Serial.begin(115200);
  delay(300);
  pwmIniciar();
  Serial.println("\n=== SIMBIO T09/T11: válvula de pinza con servo ===");
  servo(anguloCerrado);
  Serial.println("Comandos: g <°> | c | o | sc <°> | so <°> | d <ms> | r <n>");
}

void loop() {
  if (!Serial.available()) return;
  String c = Serial.readStringUntil('\n');
  c.trim();
  if (c.startsWith("g ")) servo(c.substring(2).toInt());
  else if (c == "c") servo(anguloCerrado);
  else if (c == "o") servo(anguloAbierto);
  else if (c.startsWith("sc ")) { anguloCerrado = c.substring(3).toInt(); Serial.printf("CERRADO = %d°\n", anguloCerrado); }
  else if (c.startsWith("so ")) { anguloAbierto = c.substring(3).toInt(); Serial.printf("ABIERTO = %d°\n", anguloAbierto); }
  else if (c.startsWith("d ")) { dosisMs = c.substring(2).toInt(); dosis(1); }
  else if (c.startsWith("r ")) {
    int n = c.substring(2).toInt();
    for (int i = 1; i <= n; i++) {
      dosis(i);
      if (i < n) { Serial.println("# 30 s para pesar..."); delay(30000); }
    }
  }
  Serial.printf("# CERRADO=%d° ABIERTO=%d° dosis=%lu ms\n", anguloCerrado, anguloAbierto, dosisMs);
}
