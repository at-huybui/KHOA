#include "RC522.h"
#include "LCD.h"
#include "LIB_SIM900.h"
#include "DEFINE.h"

unsigned long mil_scan_card = 0;            // Biến millis hẹn giờ quét card
unsigned long mil_check_vibrate_sensor = 0; // Biến millis hẹn giờ check sensor
unsigned long mil_unlock = 0;               // Biến millis hẹn thời gian mở khóa
unsigned long mil_turnLcd = 0;               // Biến millis hẹn thời gian mở khóa

bool _isLock = false;     // Biến lưu trạng thái cửa
bool _isTurnLcd = false;

volatile static int _countVibrate = 0;   // biến lưu số lần ngắt tạo rung động

void setup() {
  Serial.begin(9600); // Mở cổng serial debug
  System_init();  // khởi tạo hệ thông
  Lcd_init();         //Khởi tạo LCD
  //  Sim900_init();      // Khởi tạo module sim
  RF_init();          // Khởi tạo module RFID
  delay(1000);

  attachInterrupt(0, ISR_vibrate, FALLING); // Bật ngắt cho chân cảm biến rung
  Lcd_print("----WELLCOME----", 0, 0);
  doubleTick();  // còi báo
  onLcd();
}

void scanCard() {
  if (_isLock && millis() - mil_unlock > (TIME_UNLOCK + 1000) ) { //kiểm tra thời gian quét và trạng thái khóa
    String _id = RF_getID();                     //quét thẻ
    if (RF_matchId(_id)) {
      onLcd();
      tick();
      unLock();     // mở khóa
      mil_unlock = millis(); // set lại biến hẹn giờ
      _isLock = false;  // set trạng thái mở khóa
      Lcd_print("-----Unlock-----", 0, 1);
      Serial.println("-----Unlock-----");
    }
  }

  if (isOpen()) {
    if (_isLock) {
      onLcd();
      _isLock = false;
      unLock();
      tick();
      Lcd_print("-----Unlock-----", 0, 1);
      Serial.println("-----Unlock-----");
    }
    mil_unlock = millis();
  }

  if (millis() - mil_unlock > TIME_UNLOCK && !_isLock && !isOpen()) { // kiêm tra biến hẹn giờ và trạng thái khóa
    onLcd();
    doubleTick();
    lock();  // khóa
    _isLock = true; // set lại trạng thái khóa
    Lcd_print("------Lock------", 0, 1);
    Serial.println("------Lock------");
  }

  if (_isTurnLcd && millis() - mil_turnLcd > 3000) {
    Lcd_noLight();
  }
}

void onLcd() {
  _isTurnLcd = true;
  Lcd_light();
  mil_turnLcd = millis();
}

void ISR_vibrate() {
  _countVibrate++;
  Serial.println(_countVibrate);
}


void checkVibrate() {
  if (_countVibrate > 50) {
    if (isOpen()) {
      _countVibrate = 0;
    } else {
      Serial.println("*****************WARNING*******************");
      _countVibrate = 0;
      //    Sim900_sendSMS("WARNING");
      delay(1000);
    }
  }
}

void loop() {
  scanCard();
  checkVibrate();
}
