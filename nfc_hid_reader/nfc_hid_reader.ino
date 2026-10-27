/*
 * NFC HID Reader — ESP32-S3 Super Mini + Mini RC522
 * Legge l'UID dei tag NFC e lo invia come tastiera USB HID.
 */

#include <SPI.h>
#include <MFRC522.h>
#include "USB.h"
#include "USBHIDKeyboard.h"

#define SS_PIN   8
#define RST_PIN  1
#define SCK_PIN  7
#define MOSI_PIN 5
#define MISO_PIN 3

const unsigned long READ_COOLDOWN = 2000; // ms, ignora lo stesso tag

MFRC522 rfid(SS_PIN, RST_PIN);
USBHIDKeyboard Keyboard;

unsigned long lastReadTime = 0;
byte lastUID[10] = {0};
byte lastUIDLen = 0;

bool isSameAsLast() {
  if (rfid.uid.size != lastUIDLen) return false;
  for (byte i = 0; i < rfid.uid.size; i++) {
    if (rfid.uid.uidByte[i] != lastUID[i]) return false;
  }
  return true;
}

void setup() {
  USB.begin();
  Keyboard.begin();

  SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN, SS_PIN);
  rfid.PCD_Init();
  delay(500);
}

void loop() {
  if (!rfid.PICC_IsNewCardPresent()) return;
  if (!rfid.PICC_ReadCardSerial()) return;

  if (millis() - lastReadTime < READ_COOLDOWN && isSameAsLast()) {
    rfid.PICC_HaltA();
    return;
  }

  lastUIDLen = rfid.uid.size;
  memcpy(lastUID, rfid.uid.uidByte, rfid.uid.size);
  lastReadTime = millis();

  String uidStr = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    if (rfid.uid.uidByte[i] < 0x10) uidStr += "0";
    uidStr += String(rfid.uid.uidByte[i], HEX);
  }
  uidStr.toUpperCase();

  Keyboard.print(uidStr);
  Keyboard.write('\n');
  delay(20);
  Keyboard.releaseAll(); // evita tasti "incastrati"

  // PCD_StopCrypto1() omesso di proposito: causava freeze del lettore
  rfid.PICC_HaltA();
  delay(250);
}
