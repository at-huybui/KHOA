#include "LCD.h"

#define lcdVertical     2
#define lcdHorizontal   16

LiquidCrystal_I2C lcd(0x27, lcdHorizontal, lcdVertical);

void Lcd_init()
{
  lcd.init();
  delay(100);
  lcd.backlight();
}

void Lcd_gotoxy(int x, int y)
{
  lcd.setCursor(x, y);
}

void Lcd_setInt(int i, int x, int y)
{
  Lcd_gotoxy(x, y);
  lcd.print(i);
}

void Lcd_print(String st, int x, int y)
{
  Lcd_gotoxy(x, y);
  lcd.print(st);
}

void Lcd_clear()
{
  lcd.clear();
}

void Lcd_clearY(int y)
{
  Lcd_print("                    ", 0, y);
}

void Lcd_light() {
  lcd.backlight();
}

void Lcd_noLight() {
  lcd.noBacklight();
}
