/*
  SIMBIO - Tramo 7a: MQ-135 (indicador cualitativo de gases) por el ADC1
  ----------------------------------------------------------------------
  Conexiones:
    MQ-135: VCC 5 V (calefactor ~150 mA), GND.
    AO -> 1 kohm -> nodo -> GPIO 39 ;  nodo -> 1 kohm + 820 ohm (en serie) -> GND
    Capacitor de 0,1 uF entre GPIO 39 y GND.   DO sin conectar.
  ¡Antes de conectar AO al ESP32: mida con el multímetro el voltaje del nodo con el
   sensor encendido. Debe ser menor que 3,3 V!
  Uso: la primera vez deje el MQ-135 encendido 24–48 h (puede hacerse con este mismo
  sketch corriendo). Luego:
    b  = toma la línea base (aire limpio, 1 minuto)
    Acerque un algodón con alcohol o vinagre a 5 cm: el valor debe subir claramente y
    volver a la línea base al retirarlo.
  El índice 0–100 es el mismo del firmware: voltaje del módulo / 50 mV.
  Monitor serie: 115200, "Nueva línea".
*/
const int PIN_MQ = 39;
const float DIVISOR = 1820.0 / 2820.0;   // igual que DIVISOR_ANALOG del firmware

float baseMv = NAN;

float leerModuloMv(int n) {
  uint32_t acc = 0;
  for (int i = 0; i < n; i++) acc += analogReadMilliVolts(PIN_MQ);
  return (acc / (float)n) / DIVISOR;       // mV en la salida AO del módulo
}

void setup() {
  Serial.begin(115200);
  delay(300);
  analogReadResolution(12);
  analogSetPinAttenuation(PIN_MQ, ADC_11db);
  Serial.println("\n=== SIMBIO T07a: MQ-135 ===");
  Serial.println("Comando: b = tomar línea base (1 min en aire limpio)");
  Serial.println("t_s,pin_mV,modulo_mV,indice_0a100,cambio_vs_base_%");
}

void loop() {
  if (Serial.available()) {
    String c = Serial.readStringUntil('\n');
    c.trim();
    if (c == "b") {
      Serial.println(">> Tomando línea base durante 60 s...");
      float acc = 0;
      for (int i = 0; i < 60; i++) { acc += leerModuloMv(64); delay(1000); }
      baseMv = acc / 60.0;
      Serial.printf(">> Línea base = %.0f mV en el módulo\n", baseMv);
    }
  }
  float mod = leerModuloMv(64);
  float pin = mod * DIVISOR;
  float idx = constrain(mod / 50.0, 0.0, 100.0);
  Serial.printf("%lu,%.0f,%.0f,%.1f,", millis() / 1000, pin, mod, idx);
  if (isnan(baseMv)) Serial.println("-");
  else Serial.printf("%+.0f\n", 100.0 * (mod - baseMv) / baseMv);
  if (pin > 3150) Serial.println("   <- CERCA DE SATURAR EL ADC: revise el divisor");
  delay(2000);
}
