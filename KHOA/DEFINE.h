#ifndef _DEFINE_MORE_H_
#define _DEFINE_MORE_H_
#include <Arduino.h>


/* RFID PIN define  (VCC 3.3V) */
#define RST_RFID_PIN        D1  // RST pin
#define SDA_RFID_PIN        D0  // SDA pin

//  MOSI    D7
//  MISO    D6
//  SCK     D5

/* LCD PIN define ( VCC 5V) */
#define SDA_LCD_PIN         D4
#define SCL_LCD_PIN         D3

#define PIN_VIBRATE         D2     // cảm biến rung ngắt số thứ 0 tích cực cạnh xuống
#define PIN_IR              D8     //SD3     // cảm biến hồng ngoại tích cực mức thấp

#define PIN_BUZZER          D9     // loa tích cực mức thấp 
#define PIN_LOCK            D10      //SD2     //  Khóa   

#define TIME_UNLOCK         5000  // thời gian mở khóa

#define PASS_UNLOCK  "UNLOCK"

void System_init();
void unLock();
void lock();
void doubleTick();
void tick();
bool isOpen();

#endif /* _DEFINE_MORE_H_ */
