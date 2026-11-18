/*
  SIMBIO - Tramo 4: temperatura del líquido (DS18B20) y ambiente (DHT11)
  ---------------------------------------------------------------------
  Librerías (las mismas del firmware principal; Gestor de librerías del Arduino IDE):
    - OneWire (Paul Stoffregen)
    - DallasTemperature (Miles Burton)
    - DHT sensor library for ESPx (beegee_tokyo)   -> #include <DHTesp.h>
  Conexiones:
    DS18B20: rojo 3,3 V, negro GND, amarillo (DATA) GPIO 4;
             resistencia de 4,7 kohm entre DATA y 3,3 V.
    DHT11 (módulo con pull-up): VCC 3,3 V, GND, DATA GPIO 13.
  Qué debe pasar: una línea CSV cada 2 s.
    - DS18B20 = -127 -> desconectado o sin pull-up.  85 -> lectura de arranque (alimentación).
    - DHT11 "nan" -> cable o alimentación.
  Monitor serie: 115200.
*/
#include <OneWire.h>
#include <DallasTemperature.h>
#include <DHTesp.h>

const int PIN_DS18B20 = 4;
const int PIN_DHT = 13;

OneWire bus(PIN_DS18B20);
DallasTemperature ds(&bus);
DHTesp dht;

void setup() {
  Serial.begin(115200);
  delay(300);
  Serial.println("\n=== SIMBIO T04: temperaturas ===");
  ds.begin();
  ds.setResolution(12);                     // 0,0625 °C, 750 ms por conversión
  Serial.printf("Sensores DS18B20 encontrados: %d (debe ser 1)\n", ds.getDeviceCount());
  dht.setup(PIN_DHT, DHTesp::DHT11);
  Serial.println("t_s,T_liquido_C,T_ambiente_C,HR_%");
}

void loop() {
  ds.requestTemperatures();
  float tLiq = ds.getTempCByIndex(0);
  TempAndHumidity th = dht.getTempAndHumidity();
  bool dhtOk = dht.getStatus() == DHTesp::ERROR_NONE;

  Serial.printf("%lu,%.2f,%.1f,%.0f", millis() / 1000, tLiq, th.temperature, th.humidity);
  if (tLiq == DEVICE_DISCONNECTED_C) Serial.print("   <- DS18B20 DESCONECTADO");
  else if (tLiq == 85.0)             Serial.print("   <- DS18B20 85 °C: lectura no válida");
  if (!dhtOk)                        Serial.printf("   <- DHT11: %s", dht.getStatusString());
  Serial.println();
  delay(2000);                               // el DHT11 no admite más de 1 lectura/s
}
