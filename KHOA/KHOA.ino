#include "RC522.h"
#include "LCD.h"
#include "LIB_SIM900.h"
#include "DEFINE.h"

unsigned long mil_scan_card = 0;            // Biến millis hẹn giờ quét card
unsigned long mil_check_vibrate_sensor = 0; // Biến millis hẹn giờ check sensor
unsigned long mil_unlock = 0;               // Biến millis hẹn thời gian mở khóa

bool _isLock = true;     // Biến lưu trạng thái cửa

int _countVibrate = 0;   // biến lưu số lần ngắt tạo rung động
String _idStore[2] =    // Mảng lưu trữ id hợp lệ
{
  "00 00 00 00",
  "00 00 00 00"
};


void setup() {
  Serial.begin(9600); // Mở cổng serial debug
  System_init();  // khởi tạo hệ thông
  Lcd_init();         //Khởi tạo LCD
  Sim900_init();      // Khởi tạo module sim
  RF_init();          // Khởi tạo module RFID
  delay(1000);

  attachInterrupt(0, ISR_vibrate, FALLING); // Bật ngắt cho chân cảm biến rung
  Lcd_print("----Start----", 0, 0);
  doubleTick();  // còi báo
}

void scanCard() {
  if (millis() - mil_scan_card > 500 && _isLock) {  //kiểm tra thời gian quét và trạng thái khóa
    String id = RF_getID();                     //quét thẻ
    for (int i = 0; i < 2; i++) {     // kiểm tra id hợp lệ
      if (id.equals(_idStore[i])) {
        tick();
        unLock();     // mở khóa
        mil_unlock = millis(); // set lại biến hẹn giờ
        _isLock = false;  // set trạng thái mở khóa
        Lcd_print("Unlock", 1, 0);
        Serial.print("---Unlock");
      }
    }
  }

  if (millis() - mil_unlock > 3000 && !_isLock && !isUnlockIr()) { // kiêm tra biến hẹn giờ và trạng thái khóa
    doubleTick();
    lock();  // khóa
    _isLock = true; // set lại trạng thái khóa
    Lcd_print("  lock", 1, 0);
    Serial.print("---Lock");
  }
}

void checkVibrate() {
  if (_countVibrate > 5) {
    Serial.println("WARNING");
    Sim900_sendSMS("WARNING");
  }
}

void ISR_vibrate() {
  _countVibrate++;
  Serial.print("[Vibrate]"); Serial.println(_countVibrate);
}


void loop() {
  scanCard();
  checkVibrate();
}
