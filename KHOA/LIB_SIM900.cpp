#include "SIM900.h"
#include "sms.h"
#include "LIB_SIM900.h"
#include "RC522.h"

#include <SoftwareSerial.h>

// 0569125608

char numberPhone[] =  "+84329573663";  // người nhận
//char numberPhone[] =  "+84569125608";  // mynumber

SMSGSM sms; // khởi tạo đối tượng sms

boolean started = false; //biến trạng thái module sim

bool Sim900_init() {  // funtion khởi tạo module sim
  Serial.println(F("[SIM900] Khởi tạo module sim..."));
  if (gsm.begin(2400)) { // khởi động module sim
    started = true;   // thành công gán true
    if (Sim900_sendSMS("MODULE ONLINE")) {
      Serial.println(F("[SIM900] Khởi tạo thành công\n"));
    } else {
      Serial.println(F("[SIM900] Khởi tạo thất bại\n"));
    }
  } else {
    started = false;  // thất bại gán false
    Serial.println(F("\n[SIM900] status = IDLE"));
    RF_init();
  }
  return started; // trả về started
}

bool Sim900_sendSMS(char *text) { // funtion gửi tin nhắn
  Serial.println(F("[SIM900] Sending SMS ......"));
  char rt = sms.SendSMS(numberPhone, text);  // bắt đầu gửi tin nhắn với text
  Serial.print(F("[SIM900] rt =  "));  Serial.println(String(rt));
  if (rt == 1) {
    Serial.println(F("[SIM900] 1 - Gửi SMS thành công"));
    RF_init();
    return true;
  } else {
    Serial.println(F("[SIM900]  0 - Gửi SMS thất bại"));
    RF_init();
    return false;
  }
}

String Sim900_getSMS() {  // get nhận tin nhắn mới
  if (started) {    //kiểm tra trạng thái sim
    int pos = sms.IsSMSPresent(SMS_UNREAD);
    if (pos) {  //kiểm tra nêu có tin nhắn chưa đọc
      Serial.println(" pos");
      char smsContentBuf[160];// biến đệm nội dung tin nhắn
      char numberBuf[20]; // biến đệm số điện thoại format theo chuẩn quốc tế
      if (sms.GetSMS(pos, numberBuf, smsContentBuf, 160)) {
        Serial.println(F("===============[SIM900] SMS===================="));
        Serial.print("From : "); Serial.println(numberBuf);
        Serial.println(F("-----------------------------------------------"));
        Serial.println(smsContentBuf);
        Serial.println(F("-----------------------------------------------"));
        Serial.println(F("==============================================="));
        sms.SendSMS(numberBuf, "Đã Mở Khóa");
        RF_init();
        return smsContentBuf;
      }
    }
  } else {
    Serial.println(F("[SIM900] Offline"));
    Sim900_init();
  }
  return "";
}
