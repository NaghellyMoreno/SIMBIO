/*
  SIMBIO - Tramo 5: nivel del tanque con HC-SR04
  ----------------------------------------------
  Conexiones:
    HC-SR04 VCC 5 V, GND, Trig GPIO 25.
    Echo -> 680 ohm -> GPIO 34 ; GPIO 34 -> 1 kohm -> GND   (divisor: 5 V -> ~3,0 V)
  Uso (monitor serie a 115200, "Nueva línea"):
    1) En la mesa: apunte a un libro a 10, 20 y 30 cm (con regla) y compare.
    2) Sensor ya en la tapa, garrafón VACÍO: envíe  c
       -> le dice el valor de DIST_SENSOR_FONDO_CM para config.h del firmware.
    3) Agregue agua medida con una jarra (por ejemplo de 2 L en 2 L): después de cada
       adición envíe  v <litros_totales>   (ej.  v 4)
       -> calcula el área del garrafón (AREA_TANQUE_CM2 de config.h) con cada punto.
  Nivel = DIST_SENSOR_FONDO_CM - distancia medida.
*/
const int TRIG = 25, ECHO = 34;

float DIST_SENSOR_FONDO_CM = 30.0;   // <- se reemplaza con el comando "c"
const float NIVEL_MIN_CM = 19.0;     // por debajo: el calentador queda bloqueado
const float NIVEL_MAX_CM = 26.0;     // por encima: no se dosifica melaza
const float T_AIRE_C = 20.0;         // temperatura del aire bajo la tapa (aprox.)

float medirUnaVez() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(4);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  unsigned long us = pulseIn(ECHO, HIGH, 30000);
  if (us == 0) return NAN;
  float c = 331.3 + 0.606 * T_AIRE_C;          // velocidad del sonido, m/s
  return us * c / 20000.0;                     // ida y vuelta -> cm
}

// Mediana de 5 lecturas: descarta ecos sueltos y gotas de condensación
float medirCm() {
  float v[5];
  int n = 0;
  for (int i = 0; i < 5; i++) {
    float d = medirUnaVez();
    if (!isnan(d) && d >= 2.0 && d <= 400.0) v[n++] = d;
    delay(60);
  }
  if (n < 3) return NAN;
  for (int i = 0; i < n - 1; i++)
    for (int j = i + 1; j < n; j++)
      if (v[j] < v[i]) { float t = v[i]; v[i] = v[j]; v[j] = t; }
  return v[n / 2];
}

void setup() {
  Serial.begin(115200);
  delay(300);
  pinMode(TRIG, OUTPUT);
  digitalWrite(TRIG, LOW);
  pinMode(ECHO, INPUT);
  Serial.println("\n=== SIMBIO T05: nivel HC-SR04 ===");
  Serial.println("Comandos: c = calibrar fondo (vacío) | v <litros> = punto de volumen");
}

void loop() {
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    if (cmd == "c") {
      float d = medirCm();
      if (isnan(d)) Serial.println(">> Sin eco: revise el montaje.");
      else {
        DIST_SENSOR_FONDO_CM = d;
        Serial.printf(">> Copie en config.h:  #define DIST_SENSOR_FONDO_CM %.1ff\n", d);
      }
    } else if (cmd.startsWith("v ")) {
      float litros = cmd.substring(2).toFloat();
      float d = medirCm();
      float nivel = DIST_SENSOR_FONDO_CM - d;
      if (isnan(d) || nivel <= 0.5) Serial.println(">> Nivel no válido para calcular el área.");
      else Serial.printf(">> %.2f L con nivel %.1f cm -> área = %.0f cm2 (AREA_TANQUE_CM2)\n",
                         litros, nivel, litros * 1000.0 / nivel);
    }
  }

  float d = medirCm();
  if (isnan(d)) {
    Serial.println("Sin eco (¿sensor torcido, mojado o fuera de rango?)");
  } else {
    float nivel = DIST_SENSOR_FONDO_CM - d;
    Serial.printf("distancia=%.1f cm  nivel=%.1f cm", d, nivel);
    if (nivel < NIVEL_MIN_CM) Serial.print("  [BAJO: calentador bloqueado]");
    if (nivel > NIVEL_MAX_CM) Serial.print("  [ALTO]");
    Serial.println();
  }
  delay(1000);
}
