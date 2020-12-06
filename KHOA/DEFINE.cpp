#include "DEFINE.h"
#include <Arduino.h>

bool IS_DEBUG = false;

void System_init() {

  if (!IS_DEBUG) {
    pinMode(PIN_BUZZER, OUTPUT);
    pinMode(PIN_LOCK, OUTPUT);
  }

  pinMode(PIN_VIBRATE, INPUT_PULLUP);
  pinMode(PIN_IR, INPUT_PULLUP);
  digitalWrite(PIN_LOCK, HIGH);
  digitalWrite(PIN_BUZZER, HIGH);
  tick();
}

void unLock() {
  if (IS_DEBUG) {
    Serial.println(F("[DEBUG] ---UNLOCK---"));
  } else {
    digitalWrite(PIN_LOCK, HIGH);
  }
}

void lock() {
  if (IS_DEBUG) {
    Serial.println(F("[DEBUG] ----LOCK----"));
  } else {
    digitalWrite(PIN_LOCK, LOW);
  }
}

bool isOpen() {
  if (digitalRead(PIN_IR) == 1) {
    return true;
  } else {
    return false;
  }
}

void tick() {
  if (IS_DEBUG) {
    Serial.println(F("[DEBUG] ---Tick---"));
  } else {
    digitalWrite(PIN_BUZZER, LOW);
    delay(100);
    digitalWrite(PIN_BUZZER, HIGH);
  }
}

void doubleTick() {
  if (IS_DEBUG) {
    Serial.println(F("[DEBUG] ---doubleTick---"));
  } else {
    digitalWrite(PIN_BUZZER, LOW);
    delay(100);
    digitalWrite(PIN_BUZZER, HIGH);
    delay(100);
    digitalWrite(PIN_BUZZER, LOW);
    delay(100);
    digitalWrite(PIN_BUZZER, HIGH);
  }
}
