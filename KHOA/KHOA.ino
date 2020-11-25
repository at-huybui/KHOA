#include "RC522.h"
#include "LCD.h"
#include "LIB_SIM900.h"
#include "DEFINE.h"

#include <SoftwareSerial.h>

unsigned long mil_scan_card = 0;            // Biến millis hẹn giờ quét card
unsigned long mil_check_vibrate_sensor = 0; // Biến millis hẹn giờ check sensor
unsigned long mil_unlock = 0;               // Biến millis hẹn thời gian mở khóa
unsigned long mil_turnLcd = 0;               // Biến millis hẹn giờ tắt lcd
unsigned long mil_checkSMS = 0;               // Biến millis hẹn giờ check SMS

unsigned long mil_runloop = 0;               // Biến millis runloop



volatile bool _isSending = true;   // lưu trạng thái đang gửi tin nhắn
volatile bool _isLock = false;     // Biến lưu trạng thái cửa
bool _isTurnLcd = false;           // trạng thái bật lcd

volatile static byte _countVibrate = 0;   // biến lưu số lần ngắt tạo rung động

void setup() {
  Serial.begin(9600); // Mở cổng serial debug
  System_init();      // khởi tạo hệ thông
  Lcd_init();         //Khởi tạo LCD
  Lcd_print("---CONNECTING---", 0, 0);
  Sim900_init();      // Khởi tạo module sim
  attachInterrupt(0, ISR_vibrate, FALLING); // Bật ngắt cho chân cảm biến rung
  Lcd_print(F("----WELLCOME----"), 0, 0);
  doubleTick();  // còi báo
  onLcd();
  _isSending = false;
}


void loop() {
  if (!_isSending) {
    scanCard();
    checkVibrate();
    smsUnLock();
  }
}


void scanCard() {
  if (_isLock && millis() - mil_unlock > (TIME_UNLOCK + 500) ) { //kiểm tra thời gian quét và trạng thái khóa
    String _id = RF_getID();                     //quét thẻ
    if (RF_matchId(_id)) {
      Serial.print(F("[id]")); Serial.println(_id);
      _unLock();
    }
  }

  if (isOpen()) {
    if (_isLock) {
      _unLock();
    }
    mil_unlock = millis();
  }

  if (millis() - mil_unlock > TIME_LOCK && !_isLock && !isOpen()) { // kiêm tra biến hẹn giờ và trạng thái khóa
    onLcd();
    doubleTick();
    lock();  // khóa
    Lcd_print(F("------Lock------"), 0, 1);
    Serial.println(F("------Lock------"));
    _isLock = true; // set lại trạng thái khóa
  }

  if (_isTurnLcd && millis() - mil_turnLcd > 3000) {
    Lcd_noLight();
  }
}

/* Mở Khóa */
void _unLock() {
  _isLock = false;
  onLcd();
  unLock();
  tick();
  _countVibrate = 0;
  Lcd_print(F("-----Unlock-----"), 0, 1);
  mil_unlock = millis(); // set lại biến hẹn giờ
  Serial.println(F("-----Unlock-----"));
}

/* bật tắt led LCD */
void onLcd() {
  _isTurnLcd = true;
  Lcd_light();
  mil_turnLcd = millis();
}

/*kiểm tra số lần rung động của cảm biến rung */
void checkVibrate() {
  if (_countVibrate > 50) {
    if (!isOpen() && _isLock) {
      Serial.println(F("*****************WARNING*******************"));
      if (sendSMS("**WARNING**")) {
        Serial.println(F("Đã send Warning"));
        for(int i = 0; i < 10;i++){
          doubleTick();
        }
      } else {
        Serial.println(F("Chưa send Warning"));
      }
      _countVibrate = 0;
    } else {
      _countVibrate = 0;
    }
  }
}

/* Kiếm tra SMS để mở khóa */
void smsUnLock() {
  if (millis() - mil_checkSMS > 2000) {
    mil_checkSMS = millis();
    String st = Sim900_getSMS();
    if (st.equals(F(PASS_UNLOCK))) {
      _unLock();
      mil_unlock = millis(); // set lại biến hẹn giờ
    }
  }
}

/*gửi SMS */
bool sendSMS(char* msg) { 
  _isSending = true;
  if (Sim900_sendSMS(msg)) {
    Serial.println(F("Main gửi thành công"));
    _isSending = false;
    return true;
  } else {
    Serial.println(F("Main gửi thành công"));
    _isSending = false;
    return false;
  }
}


/* Hàm ngắt tại chân số 2, ngắt thứ 0 cho cảm biến rung */
void ISR_vibrate() {
  if (_isLock && !_isSending) {
    _countVibrate++;
    Serial.println(_countVibrate);
  }
}
