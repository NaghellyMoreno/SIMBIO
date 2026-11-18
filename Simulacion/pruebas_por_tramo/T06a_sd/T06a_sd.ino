/*
  SIMBIO - Tramo 6a: módulo SD solo
  ---------------------------------
  Tarjeta microSD con adaptador a SD, formateada en FAT32 (32 GB o menos).
  Conexiones del módulo SD:
    VCC 5 V (el módulo trae regulador a 3,3 V), GND,
    CS GPIO 26, SCK GPIO 18, MISO GPIO 19, MOSI GPIO 23.
  El RC522 todavía NO está conectado en esta prueba.
  Qué debe pasar: tipo y tamaño de la tarjeta, escritura de /prueba_t06.csv,
  lectura de las mismas 10 líneas y "PRUEBA SD: OK".
  Monitor serie: 115200.
*/
#include <SPI.h>
#include <SD.h>

const int SD_CS = 26, SPI_SCK = 18, SPI_MISO = 19, SPI_MOSI = 23;
const int RFID_SS = 5;   // se deja en alto aunque el RC522 no esté conectado

void setup() {
  Serial.begin(115200);
  delay(300);
  Serial.println("\n=== SIMBIO T06a: módulo SD ===");
  pinMode(RFID_SS, OUTPUT); digitalWrite(RFID_SS, HIGH);
  pinMode(SD_CS, OUTPUT);   digitalWrite(SD_CS, HIGH);
  SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);

  if (!SD.begin(SD_CS, SPI, 4000000)) {      // 4 MHz, igual que el firmware
    Serial.println("FALLA: no se inició la SD. Revise cables, formato FAT32 y 5 V del módulo.");
    return;
  }
  const char* tipos[] = {"ninguna", "MMC", "SD", "SDHC", "desconocida"};
  uint8_t tipo = SD.cardType();
  Serial.printf("Tarjeta: %s, %llu MB\n", tipos[tipo > 4 ? 4 : tipo], SD.cardSize() / (1024ULL * 1024ULL));

  File f = SD.open("/prueba_t06.csv", FILE_WRITE);
  if (!f) { Serial.println("FALLA: no se pudo crear el archivo."); return; }
  f.println("n,ms");
  for (int i = 1; i <= 10; i++) f.printf("%d,%lu\n", i, millis());
  f.close();

  f = SD.open("/prueba_t06.csv");
  int lineas = 0;
  while (f.available()) { if (f.read() == '\n') lineas++; }
  f.close();
  Serial.printf("Líneas leídas: %d (esperadas 11)\n", lineas);
  Serial.println(lineas == 11 ? "PRUEBA SD: OK" : "PRUEBA SD: FALLA");
}

void loop() {}
