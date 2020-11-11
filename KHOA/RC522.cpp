#include "RC522.h"
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         9           // Configurable, see typical pin layout above
#define SS_PIN          10          // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN); 

void RF_init(){
  Serial.println("[RFID] init.");
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println("[RFID] sẵn sàng đọc thẻ. ");
}

String RF_getID() {
  // Sẵn sàng cho việc đọc thẻ
  if ( ! mfrc522.PICC_IsNewCardPresent()) { //If a new PICC placed to RFID reader continue
    Serial.println("[RFID] false - IsNewCardPresent");
    return ""; // trả về rỗng
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) {   //Since a PICC placed get Serial and continue
    Serial.println("[RFID] false - ReadCardSerial");
    return ""; // trả về rỗng
  }
  byte uIDbuffer[4];
  String stringID = "";
  for ( uint8_t i = 0; i < 4; i++) {  // scanf 4 byte UID của thẻ
    uIDbuffer[i] = mfrc522.uid.uidByte[i]; // đọc UID lưu vào mảng đệm
    stringID.concat(String(mfrc522.uid.uidByte[i], HEX)); // chuyển mảng đệm sang chuỗi để trả về
  }
  stringID.toUpperCase();
  mfrc522.PICC_HaltA(); // Stop reading
  
  Serial.print("Card UID : "); 
  Serial.println("");
  
  return stringID;
}
