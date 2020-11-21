#ifndef _DEFINE_MORE_H_
#define _DEFINE_MORE_H_
#include <Arduino.h>

#if defined(ARDUINO_AVR_MEGA2560)

/* define for MEGA board */
#define PIN_VIBRATE         2     // cảm biến rung ngắt số thứ 0 tích cực cạnh xuống
#define PIN_BUZZER          5     // loa tích cực mức thấp 
#define PIN_LOCK            6     //  Khóa   
#define PIN_IR              7     // cảm biến hồng ngoại tích cực mức thấp
#define RST_PIN             9     // RST pin của RFID
#define SDA_PIN             53    // SDA pin của RFID
#define LCD_ADRESS          0x27  // Uno

#else

/* define for UNO board and NANO board */
#define PIN_VIBRATE         2     // cảm biến rung ngắt số thứ 0 tích cực cạnh xuống
#define PIN_BUZZER          5     // loa tích cực mức thấp 
#define PIN_LOCK            6     //  Khóa   
#define PIN_IR              7     // cảm biến hồng ngoại tích cực mức thấp
#define RST_PIN             9     // RST pin của RFID
#define SDA_PIN             10    // SDA pin của RFID
#define LCD_ADRESS          0x27  // Uno

#endif

#define TIME_UNLOCK         5000  // thời gian mở khóa
#define TIME_LOCK         3000  // thời gian mở khóa

#define PASS_UNLOCK  "UNLOCK"

void System_init();
void unLock();
void lock();
void doubleTick();
void tick();
bool isOpen();

#endif /* _DEFINE_MORE_H_ */
