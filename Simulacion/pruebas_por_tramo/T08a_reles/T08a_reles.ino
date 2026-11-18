/*
  SIMBIO - Tramo 8a: relés SIN 110 V (solo el lado de 5 V)
  --------------------------------------------------------
  Conexiones de cada módulo de relé (Songle SRD-05VDC-SL-C, 1 canal):
    VCC 5 V, GND, IN -> GPIO 21 (bomba de aire) / GPIO 22 (calentador)
    Bornes COM/NO/NC: NADA conectado en esta prueba.
  Comandos (monitor serie a 115200, "Nueva línea"):
    a = cambia relé de aire   c = cambia relé del calentador   x = ambos apagados
    p = invierte la polaridad (para averiguar si el módulo es activo en alto o en bajo)
  Qué averiguar:
    1) ¿Con qué polaridad el relé "clica" y su LED enciende cuando el monitor dice ON?
       -> Ese valor va en RELE_ACTIVO_ALTO de config.h (1 = alto, 0 = bajo).
    2) Con el multímetro en continuidad entre COM y NO: debe sonar solo en ON.
    3) Al reiniciar el ESP32 (botón EN) los relés NO deben clicar ni quedar encendidos.
  Si el relé no se apaga nunca con el ESP32 (3,3 V no alcanza a apagar el optoacoplador
  de algunos módulos de 5 V): ver la guía, tramo 8 (transistor 2N2222 por relé).
*/
const int RELE_AIRE = 21, RELE_CAL = 22;
bool activoAlto = true;
bool aire = false, cal = false;

void aplicar() {
  digitalWrite(RELE_AIRE, (aire == activoAlto) ? HIGH : LOW);
  digitalWrite(RELE_CAL,  (cal  == activoAlto) ? HIGH : LOW);
  Serial.printf("Polaridad: activo en %s | AIRE %s | CALENTADOR %s\n",
                activoAlto ? "ALTO" : "BAJO", aire ? "ON" : "off", cal ? "ON" : "off");
}

void setup() {
  // Fijar el nivel de "apagado" ANTES de configurar el pin como salida evita un clic al arrancar
  digitalWrite(RELE_AIRE, activoAlto ? LOW : HIGH);
  digitalWrite(RELE_CAL,  activoAlto ? LOW : HIGH);
  pinMode(RELE_AIRE, OUTPUT);
  pinMode(RELE_CAL, OUTPUT);
  Serial.begin(115200);
  delay(300);
  Serial.println("\n=== SIMBIO T08a: relés (sin 110 V) ===");
  aplicar();
}

void loop() {
  if (!Serial.available()) return;
  String c = Serial.readStringUntil('\n');
  c.trim();
  if (c == "a") aire = !aire;
  else if (c == "c") cal = !cal;
  else if (c == "x") aire = cal = false;
  else if (c == "p") { activoAlto = !activoAlto; aire = cal = false; }
  else return;
  aplicar();
}
