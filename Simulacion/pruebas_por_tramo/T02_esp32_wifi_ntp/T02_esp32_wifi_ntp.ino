/*
  SIMBIO - Tramo 2: ESP32 solo (vida, wifi y hora NTP)
  -----------------------------------------------------
  Qué prueba:
    - Que el ESP32 se programa y arranca.
    - Que se conecta al wifi (o a los datos del celular compartidos).
    - Que obtiene la hora por NTP en zona horaria de Colombia (UTC-5).
  Conexiones: ninguna (solo USB). El LED verde (GPIO 2) puede ser el LED de la placa.
  Monitor serie: 115200 baudios.

  ANTES DE CARGAR: escriba el nombre y la clave de su red abajo.
*/
#include <WiFi.h>
#include <time.h>

const char* WIFI_SSID = "NOMBRE_DE_SU_RED";
const char* WIFI_PASS = "CLAVE_DE_SU_RED";

const int LED_VERDE = 2;
const char* ZONA_HORARIA = "<-05>5";   // Colombia, sin horario de verano

void conectarWifi() {
  Serial.printf("Conectando a \"%s\"", WIFI_SSID);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  unsigned long t0 = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - t0 < 20000) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  if (WiFi.status() == WL_CONNECTED) {
    Serial.printf("OK  IP: %s  Señal (RSSI): %d dBm\n",
                  WiFi.localIP().toString().c_str(), WiFi.RSSI());
    Serial.println("    RSSI mejor que -70 dBm = buena señal en el sitio del tanque.");
  } else {
    Serial.println("FALLA: no se conectó en 20 s. Revise nombre/clave y que la red sea de 2,4 GHz.");
  }
}

void sincronizarHora() {
  configTzTime(ZONA_HORARIA, "pool.ntp.org", "time.google.com");
  struct tm t;
  if (getLocalTime(&t, 10000)) {
    char buf[32];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &t);
    Serial.printf("OK  Hora NTP: %s\n", buf);
  } else {
    Serial.println("FALLA: no se obtuvo la hora NTP (¿hay internet?).");
  }
}

void setup() {
  Serial.begin(115200);
  delay(500);
  pinMode(LED_VERDE, OUTPUT);
  Serial.println("\n=== SIMBIO T02: ESP32, wifi y NTP ===");
  Serial.printf("Chip: %s  Núcleos: %d  MHz: %d\n",
                ESP.getChipModel(), ESP.getChipCores(), ESP.getCpuFreqMHz());
  conectarWifi();
  if (WiFi.status() == WL_CONNECTED) sincronizarHora();
}

void loop() {
  static unsigned long ultimo = 0;
  digitalWrite(LED_VERDE, (millis() / 500) % 2);   // parpadeo = el programa está vivo

  if (millis() - ultimo >= 10000) {
    ultimo = millis();
    struct tm t;
    char buf[32] = "sin hora";
    if (getLocalTime(&t, 100)) strftime(buf, sizeof(buf), "%H:%M:%S", &t);
    Serial.printf("[%lus] hora=%s  wifi=%s  RSSI=%d  memoria libre=%u B\n",
                  millis() / 1000, buf,
                  WiFi.status() == WL_CONNECTED ? "si" : "NO",
                  WiFi.RSSI(), ESP.getFreeHeap());
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("Wifi caído: reintentando...");
      WiFi.reconnect();
    }
  }
}
