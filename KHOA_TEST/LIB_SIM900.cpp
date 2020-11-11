#include "SIM900.h"
#include "sms.h"
#include "LIB_SIM900.h"

char numberPhone[] =  "+84329573663";  // số điện thoại người nhận
String SIM_NUMBER = "+84329573xxx";     // số điện thoại của sim

SMSGSM sms; // khởi tạo đối tượng sms

boolean started = false; //biến trạng thái module sim

bool Sim900_init() {  // funtion khởi tạo module sim
  Serial.println("[SIM900] Khởi tạo module sim...");
  if (gsm.begin(2400)) { // khởi động module sim
    Serial.println("\n[SIM900] status = __READY__");
    started = true;   // thành công gán true
    sms.SendSMS(numberPhone, "Module Online");
  } else {
    started = false;  // thất bại gán false
    Serial.println("\n[SIM900] status = IDLE");
  }
  return started; // trả về started
}

bool Sim900_sendSMS(char *text) { // funtion gửi tin nhắn
  char rt = sms.SendSMS(numberPhone, text);  // bắt đầu gửi tin nhắn với text
  switch (rt) { // kiểm tra trạng thái gửi
    case -1: {
        Serial.println("[SIM900] -1 - dấu phẩy. đường đến mô-đun GSM không miễn phí");
        return false;
        break;
      }
    case -2: {
        Serial.println("[SIM900]  -2 - Mô-đun GSM không trả lời trong thời gian chờ");
        return false;
        break;
      }
    case -3: {
        Serial.println("[SIM900] -3 - Mô-đun GSM đã trả lời chuỗi ");
        return false;
        break;
      }
    case 0: {
        Serial.println("[SIM900]  0 - Gửi SMS thất bại");
        return false;
        break;
      }
    case 1: {
        Serial.println("[SIM900] 1 - Gửi SMS thành công");
        return true;
        break;
      }
  }
}

String Sim900_getSMS() {  // get nhận tin nhắn mới
  if (started) {    //kiểm tra trạng thái sim
    int pos = sms.IsSMSPresent(SMS_UNREAD);
    if (pos) {  //kiểm tra nêu có tin nhắn chưa đọc
      char smsContentBuf[160];// biến đệm nội dung tin nhắn
      char numberBuf[20]; // biến đệm số điện thoại format theo chuẩn quốc tế
      if (sms.GetSMS(pos, numberBuf, smsContentBuf, 160)) {
        Serial.print("[SIM900] Số điện thoại người gửi: ");
        Serial.println(numberBuf);
        Serial.println("[SIM900] Nội dung tin nhắn: ");
        Serial.println(smsContentBuf);
        //sms.SendSMS(number, "Da doc tin");
        Serial.println("[SIM900] Đã đọc tin nhắn");
        return smsContentBuf;
      }
    }
    delay(1000);
  } else {
    Serial.println("[SIM900] Offline");
    Sim900_init();
  }
  return "";
}
