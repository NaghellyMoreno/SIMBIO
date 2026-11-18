/*
  SIMBIO - Tramo 6c: RC522 y módulo SD juntos en el mismo bus SPI
  ---------------------------------------------------------------
  Conexiones: las de T06a y T06b a la vez (SCK 18, MISO 19, MOSI 23 compartidos;
  CS del RC522 = 5, CS de la SD = 26).
  Qué debe pasar:
    - Cada tarjeta leída se agrega a /lotes_t06.csv en la SD.
    - Cada 5 s se revisa que el RC522 siga respondiendo (versión 0x91/0x92).
    - Deje correr 10 minutos pasando tarjetas: no debe aparecer "RC522 PERDIDO".
  Si el RC522 deja de responder solo cuando la SD está conectada: el módulo SD no
  libera MISO. Solución documentada: mover la SD al segundo bus SPI (HSPI).
  Monitor serie: 115200.
*/
#include <SPI.h>
#include <SD.h>
#include <MFRC522.h>

const int RFID_SS = 5, RFID_RST = 27, SD_CS = 26;
MFRC522 rfid(RFID_SS, RFID_RST);
bool sdOk = false;
unsigned long lecturas = 0, fallosRfid = 0;

bool rfidResponde() {
  byte v = rfid.PCD_ReadRegister(MFRC522::VersionReg);
  return v != 0x00 && v != 0xFF;
}

void setup() {
  Serial.begin(115200);
  delay(300);
  Serial.println("\n=== SIMBIO T06c: RC522 + SD en el mismo SPI ===");
  pinMode(RFID_SS, OUTPUT); digitalWrite(RFID_SS, HIGH);   // ambos CS en alto antes de iniciar
  pinMode(SD_CS, OUTPUT);   digitalWrite(SD_CS, HIGH);
  SPI.begin(18, 19, 23);

  sdOk = SD.begin(SD_CS, SPI, 4000000);
  Serial.printf("SD: %s\n", sdOk ? "OK" : "FALLA");
  rfid.PCD_Init();
  Serial.printf("RC522: %s\n", rfidResponde() ? "OK" : "FALLA");
  if (sdOk && !SD.exists("/lotes_t06.csv")) {
    File f = SD.open("/lotes_t06.csv", FILE_WRITE);
    if (f) { f.println("ms,uid"); f.close(); }
  }
  Serial.println("Pase tarjetas durante 10 minutos...");
}

void loop() {
  static unsigned long tRev = 0;
  if (millis() - tRev > 5000) {
    tRev = millis();
    if (!rfidResponde()) {
      fallosRfid++;
      Serial.println("RC522 PERDIDO: reiniciando el lector");
      rfid.PCD_Init();
    }
    Serial.printf("[%lus] lecturas guardadas=%lu  fallos RC522=%lu  SD=%s\n",
                  millis() / 1000, lecturas, fallosRfid, sdOk ? "OK" : "no");
  }

  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) { delay(50); return; }
  char uid[21] = "";
  for (byte i = 0; i < rfid.uid.size && i < 10; i++) sprintf(uid + 2 * i, "%02X", rfid.uid.uidByte[i]);
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();

  if (sdOk) {
    File f = SD.open("/lotes_t06.csv", FILE_APPEND);
    if (f) { f.printf("%lu,%s\n", millis(), uid); f.close(); lecturas++; }
    else Serial.println("No se pudo escribir en la SD");
  }
  Serial.printf("UID %s guardado\n", uid);
}
