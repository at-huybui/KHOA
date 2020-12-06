#include "RC522.h"

String _idStore[2] =    // Mảng lưu trữ id hợp lệ
{
  "43 60 48 3E",
  "99 BE 1B C9"
};

MFRC522 mfrc522(SDA_RFID_PIN, RST_RFID_PIN);  // Create MFRC522 instance

/* Nguyên Mẫu hàm */
String toHex(char data);


/* khởi tạo RC522 */
void RF_init() {
  Serial.println(F("[RFID] Đang kết nối..."));
  SPI.begin();      // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522
  delay(50);       // Optional delay. Some board do need more time after init to be ready, see Readme
  mfrc522.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card Reader details
  Serial.println(F("[RFID] Sẵn sàng quét thẻ"));
}

/* Lấy UID của thẻ */
String RF_getID() {
  if ( mfrc522.PICC_IsNewCardPresent()) {
    Serial.println(F("[RFID] Thấy thẻ"));
    if (mfrc522.PICC_ReadCardSerial()) {
      String _uid = "";
      for (byte i = 0; i < mfrc522.uid.size; i++) {
        _uid += toHex(mfrc522.uid.uidByte[i]);
        if (i < mfrc522.uid.size - 1) {
          _uid += " ";
        }
      }
      Serial.print(F("[RFID] Card ID: ")); Serial.println(_uid);
      mfrc522.PICC_HaltA(); // Already done if it was a MIFARE Classic PICC.
      return _uid;
    }
  }
  return "";
}

bool RF_matchId(String id_) {
  for (int i = 0; i < 2; i++) {     // kiểm tra id hợp lệ
    if (id_.equals(_idStore[i])) {
      Serial.print(F("[RFID] --MATCH-- ")); Serial.print(id_); Serial.println(F(" --MATCH--"));
      return true;
    }
  }
  return false;
}

// chuyển đổi int to hexa
String toHex(char data) {
  String st = "";
  char const hex_chars[16] =
  {
    '0', '1', '2', '3',
    '4', '5', '6', '7',
    '8', '9', 'A', 'B',
    'C', 'D', 'E', 'F'
  };
  st += hex_chars[ ( data & 0xF0 ) >> 4 ];
  st += hex_chars[ ( data & 0x0F ) >> 0 ];
  return st;
}
