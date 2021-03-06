#include "DEFINE.h"

void System_init(){
  pinMode(PIN_BUZZER,OUTPUT);
  pinMode(PIN_LOCK,OUTPUT);
  pinMode(PIN_VIBRATE,INPUT_PULLUP);
  pinMode(PIN_IR,INPUT_PULLUP);

  digitalWrite(PIN_LOCK,LOW);
}

void unLock(){
  digitalWrite(PIN_LOCK,HIGH);
}

void lock(){
  digitalWrite(PIN_LOCK,LOW);
}

bool isUnlockIr(){
  if(digitalRead(PIN_IR) == 1){
    return true;
  }else {
    return false;
  }
}

void tick() {
  digitalWrite(PIN_BUZZER, HIGH);
  delay(100);
  digitalWrite(PIN_BUZZER, LOW);
}

void doubleTick() {
  digitalWrite(PIN_BUZZER, HIGH);
  delay(100);
  digitalWrite(PIN_BUZZER, LOW);
  delay(100);
  digitalWrite(PIN_BUZZER, HIGH);
  delay(100);
  digitalWrite(PIN_BUZZER, LOW);
}
