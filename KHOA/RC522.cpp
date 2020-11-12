#include "RC522.h"

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

String toHex(char data);

String RF_getID() {
  if ( mfrc522.PICC_IsNewCardPresent()) {
    if (mfrc522.PICC_ReadCardSerial()) {
      String _uid = "";
      for (byte i = 0; i < mfrc522.uid.size; i++) {
        _uid += toHex(mfrc522.uid.uidByte[i]);
        if(i < mfrc522.uid.size - 1){
          _uid += " ";
        }
      }
      Serial.print("Card ID: "); Serial.println(_uid);
      mfrc522.PICC_HaltA(); // Already done if it was a MIFARE Classic PICC.
      return _uid;
    }
  }
  return "";
}

void RF_init() {
  SPI.begin();      // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522
  delay(4);       // Optional delay. Some board do need more time after init to be ready, see Readme
  mfrc522.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card Reader details
  Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
}

String toHex(char data) {
  String st = "";
  char const hex_chars[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
  st += hex_chars[ ( data & 0xF0 ) >> 4 ];
  st += hex_chars[ ( data & 0x0F ) >> 0 ];
  return st;
}
