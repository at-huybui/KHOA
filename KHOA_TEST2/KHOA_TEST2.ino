#include "RC522.h"

//
//#include <SPI.h>
//#include <MFRC522.h>
//
//#define RST_PIN         5          // Configurable, see typical pin layout above
//#define SS_PIN          53         // Configurable, see typical pin layout above
//
//MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance
//
//String currentCard = "";

void setup() {
  Serial.begin(9600);   // Initialize serial communications with the PC
  while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  initRFID();

}

void loop() {
  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  getID();

}

//String getID() {
//  if ( mfrc522.PICC_IsNewCardPresent()) {
//    if (mfrc522.PICC_ReadCardSerial()) {
//      String _uid = "";
//      for (byte i = 0; i < mfrc522.uid.size; i++) {
//        _uid += toHex(mfrc522.uid.uidByte[i]);
//      }
//      Serial.print("Card ID: "); Serial.println(_uid);
//      currentCard = _uid;
//      mfrc522.PICC_HaltA(); // Already done if it was a MIFARE Classic PICC.
//      return _uid;
//    }
//  }
//  return "";
//}
//
//void initRFID() {
//  SPI.begin();      // Init SPI bus
//  mfrc522.PCD_Init();   // Init MFRC522
//  delay(4);       // Optional delay. Some board do need more time after init to be ready, see Readme
//  mfrc522.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card Reader details
//  Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
//}
//
//String toHex(char data) {
//  String st = "";
//  char const hex_chars[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
//  st += hex_chars[ ( data & 0xF0 ) >> 4 ];
//  st += hex_chars[ ( data & 0x0F ) >> 0 ];
//  return st;
//}
