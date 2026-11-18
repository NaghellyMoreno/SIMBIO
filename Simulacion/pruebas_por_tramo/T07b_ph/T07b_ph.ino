/*
  SIMBIO - Tramo 7b: pH (PH-4502C + E201) por el ADC1, con prueba de 2 puntos
  ---------------------------------------------------------------------------
  Librerías: OneWire y DallasTemperature (para la temperatura del buffer).
  Conexiones:
    PH-4502C: V+ 5 V, G GND (las dos patas G), Po -> divisor -> GPIO 36. Do y To libres.
    Po -> 1 kohm -> nodo -> GPIO 36 ;  nodo -> 1 kohm + 820 ohm (en serie) -> GND
    Capacitor de 0,1 uF entre GPIO 36 y GND.
    DS18B20 como en el tramo 4 (GPIO 4 con 4,7 kohm a 3,3 V).
  PASO 0 (con multímetro, antes de este sketch): desconecte la sonda, ponga en corto
    el centro del BNC con su carcasa y gire el potenciómetro de OFFSET hasta que Po
    marque 2,50 V. (El otro potenciómetro es el umbral de Do: no se usa.)
  Uso (monitor serie a 115200, "Nueva línea"):
    Sonda en buffer pH 7 (con el DS18B20 en el mismo vaso). Espere a que diga ESTABLE.
      7  -> guarda el punto de pH 7
    Enjuague con agua destilada, seque con papel sin frotar, sonda en buffer pH 4.
      4  -> guarda el punto de pH 4 y calcula la pendiente
    Luego vuelva a pH 7: debe leer 7,00 ± 0,10.
  Esta prueba SOLO verifica el hardware. La calibración que usa el lote se hace
  después en el firmware principal con el DIP de calibración (tramo 9).
*/
#include <OneWire.h>
#include <DallasTemperature.h>

const int PIN_PH = 36, PIN_DS = 4;
const float DIVISOR = 1820.0 / 2820.0;   // igual que DIVISOR_ANALOG del firmware
const float PEND_MIN = 100.0, PEND_MAX = 250.0;   // |mV/pH| aceptable (igual que config.h)

OneWire bus(PIN_DS);
DallasTemperature ds(&bus);

float v7 = NAN, v4 = NAN, tCal = 25.0;
float historial[15];          // una muestra cada 2 s = 30 s
int nHist = 0;

float leerModuloMv() {
  uint32_t acc = 0;
  for (int i = 0; i < 100; i++) acc += analogReadMilliVolts(PIN_PH);
  return (acc / 100.0) / DIVISOR;          // mV en Po del módulo
}

float leerT() {
  ds.requestTemperatures();
  float t = ds.getTempCByIndex(0);
  return (t == DEVICE_DISCONNECTED_C) ? NAN : t;
}

// Misma fórmula del firmware: 2 puntos + compensación de Nernst
float calcularPh(float mv, float t) {
  float pend = (v4 - v7) / (4.0 - 7.0);
  float factor = ((isnan(t) ? 25.0 : t) + 273.15) / (tCal + 273.15);
  return 7.0 + (mv - v7) / (pend * factor);
}

void setup() {
  Serial.begin(115200);
  delay(300);
  analogReadResolution(12);
  analogSetPinAttenuation(PIN_PH, ADC_11db);
  ds.begin();
  Serial.println("\n=== SIMBIO T07b: pH ===");
  Serial.println("Comandos: 7 = punto pH 7 | 4 = punto pH 4");
}

void loop() {
  float mv = leerModuloMv();
  float t = leerT();

  // Estabilidad: cambio en los últimos 30 s
  if (nHist < 15) historial[nHist++] = mv;
  else { for (int i = 0; i < 14; i++) historial[i] = historial[i + 1]; historial[14] = mv; }
  float deriva = (nHist == 15) ? fabs(historial[14] - historial[0]) : 999;
  bool estable = deriva < 3.0;             // < 3 mV en 30 s ≈ 0,02 pH

  if (Serial.available()) {
    String c = Serial.readStringUntil('\n');
    c.trim();
    if (c == "7") {
      v7 = mv; tCal = isnan(t) ? 25.0 : t;
      Serial.printf(">> pH 7 = %.0f mV a %.1f °C%s\n", v7, tCal, estable ? "" : "  (AÚN NO ESTABA ESTABLE)");
    } else if (c == "4") {
      if (isnan(v7)) Serial.println(">> Primero el punto de pH 7.");
      else {
        v4 = mv;
        float pend = fabs((v4 - v7) / 3.0);
        Serial.printf(">> pH 4 = %.0f mV.  Pendiente = %.1f mV/pH\n", v4, pend);
        if (v4 <= v7) Serial.println(">> RECHAZADA: con este módulo el pH 4 debe dar MÁS voltaje que el pH 7.");
        else if (pend < PEND_MIN || pend > PEND_MAX)
          Serial.printf(">> RECHAZADA: esperado %.0f a %.0f mV/pH. Sonda vieja, sucia o buffers dañados.\n", PEND_MIN, PEND_MAX);
        else Serial.println(">> ACEPTADA. Vuelva a pH 7 y compruebe que lea 7,00 ± 0,10.");
      }
    }
  }

  Serial.printf("Po=%.0f mV  pin=%.0f mV  T=%.2f °C  deriva30s=%.1f mV %s",
                mv, mv * DIVISOR, t, deriva < 999 ? deriva : 0.0, estable ? "ESTABLE" : "...");
  if (!isnan(v7) && !isnan(v4) && v4 > v7) Serial.printf("  pH=%.2f", calcularPh(mv, t));
  Serial.println();
  if (mv * DIVISOR > 3150 || mv * DIVISOR < 50) Serial.println("   <- fuera de rango: sonda o divisor");
  delay(1000);   // + ~1 s de lectura del DS18B20 = una línea cada ~2 s
}
