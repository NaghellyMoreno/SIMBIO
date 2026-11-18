/*
  SIMBIO - Tramo 3: interfaz local (LEDs, buzzer, pulsadores y DIP)
  -----------------------------------------------------------------
  Conexiones:
    LED rojo     GPIO 16 -> 330 ohm -> ánodo LED, cátodo a GND
    LED amarillo GPIO 17 -> 330 ohm -> LED -> GND
    LED verde    GPIO 2  -> 330 ohm -> LED -> GND
    Buzzer KY-012 (activo): "S" a 5 V, "-" al colector del 2N2222;
                  emisor a GND; base <- 1 kohm <- GPIO 15
    Pulsador LOTE GPIO 32 a GND (pull-up interno)
    Pulsador OK   GPIO 33 a GND (pull-up interno)
    DIP calibración GPIO 35: 10 kohm de GPIO 35 a 3,3 V; interruptor de GPIO 35 a GND
  Qué debe pasar:
    - Al arrancar: rojo, amarillo, verde y un pitido corto.
    - Pulsar LOTE -> rojo encendido mientras se mantiene + pitido corto.
    - Pulsar OK   -> amarillo encendido mientras se mantiene + pitido corto.
    - DIP en ON (GPIO 35 a GND) -> verde encendido y "MODO CALIBRACION" en el monitor.
  Monitor serie: 115200.
*/
const int LED_ROJO = 16, LED_AMARILLO = 17, LED_VERDE = 2;
const int BUZZER = 15;
const int BTN_LOTE = 32, BTN_OK = 33, DIP_CAL = 35;

void pitido(int ms) {
  digitalWrite(BUZZER, HIGH);
  delay(ms);
  digitalWrite(BUZZER, LOW);
}

// Lectura con antirrebote: devuelve true una sola vez por pulsación
bool pulsado(int pin, bool &estadoAnterior, unsigned long &tCambio) {
  bool ahora = digitalRead(pin) == LOW;      // LOW = pulsado
  if (ahora != estadoAnterior && millis() - tCambio > 40) {
    tCambio = millis();
    estadoAnterior = ahora;
    return ahora;
  }
  return false;
}

void setup() {
  Serial.begin(115200);
  delay(300);
  pinMode(LED_ROJO, OUTPUT);
  pinMode(LED_AMARILLO, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, LOW);
  pinMode(BTN_LOTE, INPUT_PULLUP);
  pinMode(BTN_OK, INPUT_PULLUP);
  pinMode(DIP_CAL, INPUT);                   // GPIO 35 no tiene pull-up interno

  Serial.println("\n=== SIMBIO T03: interfaz local ===");
  int leds[] = {LED_ROJO, LED_AMARILLO, LED_VERDE};
  for (int i = 0; i < 3; i++) {
    digitalWrite(leds[i], HIGH);
    delay(400);
    digitalWrite(leds[i], LOW);
  }
  pitido(150);
  Serial.println("Si vio los 3 LEDs y oyó el pitido, la parte de salidas está bien.");
}

void loop() {
  static bool antLote = false, antOk = false;
  static unsigned long tLote = 0, tOk = 0;
  static int dipAnterior = -1;

  if (pulsado(BTN_LOTE, antLote, tLote)) { Serial.println("Pulsador LOTE"); pitido(60); }
  if (pulsado(BTN_OK, antOk, tOk))       { Serial.println("Pulsador OK");   pitido(60); }
  digitalWrite(LED_ROJO, antLote);
  digitalWrite(LED_AMARILLO, antOk);

  int dip = digitalRead(DIP_CAL) == LOW;     // ON = a GND
  if (dip != dipAnterior) {
    dipAnterior = dip;
    Serial.println(dip ? "DIP: MODO CALIBRACION" : "DIP: modo operación");
  }
  digitalWrite(LED_VERDE, dip);
  delay(5);
}
