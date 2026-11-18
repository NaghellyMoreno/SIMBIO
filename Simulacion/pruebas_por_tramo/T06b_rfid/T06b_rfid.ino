/*
  SIMBIO - Tramo 6b: lector RFID RC522 solo
  -----------------------------------------
  Librería: MFRC522 (GithubCommunity / miguelbalboa)
  Conexiones del RC522 (¡SOLO 3,3 V!):
    3.3V -> 3,3 V del ESP32, GND, SDA(SS) GPIO 5, RST GPIO 27,
    SCK GPIO 18, MISO GPIO 19, MOSI GPIO 23, IRQ sin conectar.
  Desconecte el módulo SD para esta prueba (o deje su CS en alto).
  Qué debe pasar:
    - "Versión del chip: 0x91" o "0x92" (0x00 o 0xFF = sin comunicación).
    - Al acercar una tarjeta o llavero: su UID. Anote el UID de cada uno con un
      marcador en la tarjeta (Lote 1, Lote 2, Lote 3).
  Monitor serie: 115200.
*/
#include <SPI.h>
#include <MFRC522.h>

const int RFID_SS = 5, RFID_RST = 27, SD_CS = 26;
MFRC522 rfid(RFID_SS, RFID_RST);

void setup() {
  Serial.begin(115200);
  delay(300);
  Serial.println("\n=== SIMBIO T06b: RFID RC522 ===");
  pinMode(SD_CS, OUTPUT); digitalWrite(SD_CS, HIGH);
  SPI.begin(18, 19, 23, RFID_SS);
  rfid.PCD_Init();
  delay(50);
  byte v = rfid.PCD_ReadRegister(MFRC522::VersionReg);
  Serial.printf("Versión del chip: 0x%02X  %s\n", v,
                (v == 0x00 || v == 0xFF) ? "-> SIN COMUNICACIÓN: revise SPI y 3,3 V" : "-> OK");
  Serial.println("Acerque una tarjeta o llavero...");
}

void loop() {
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) { delay(50); return; }
  Serial.print("UID: ");
  for (byte i = 0; i < rfid.uid.size; i++) Serial.printf("%02X", rfid.uid.uidByte[i]);
  Serial.printf("   tipo: %s\n", rfid.PICC_GetTypeName(rfid.PICC_GetType(rfid.uid.sak)));
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}
