#ifndef _DEFINE_MORE_H_
#define _DEFINE_MORE_H_
#include <Arduino.h>


#define PIN_VIBRATE         2   // tích cực mức thấp 
#define INTERRUPT_VIBRATE   0   // Ngắt chân 2
#define PIN_BUZZER          5   // tích cực mức thấp 
#define PIN_LOCK            6  // tích cực mức thấp 
#define PIN_IR              7  // tích cực mức thấp 

void System_init();
void unLock();
void lock();
void doubleTick();
void tick();
bool isUnlockIr();

#endif /* _DEFINE_MORE_H_ */
