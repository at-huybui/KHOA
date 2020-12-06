#ifndef _LCD_FUNTION_H_
#define _LCD_FUNTION_H_
#include <Arduino.h>

#include "DEFINE.h"

#include <LiquidCrystal_I2C.h>
#include <Wire.h>

void Lcd_init();
void Lcd_gotoxy(int x, int y);
void Lcd_setInt(int i, int x, int y);
void Lcd_print(String st, int x, int y);
void Lcd_clear();
void Lcd_clearY(int y);
void Lcd_noLight();
void Lcd_light();

#endif
