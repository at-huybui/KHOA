#include "LCD.h"

#define lcdVertical     4
#define lcdHorizontal   20
#define lcdAdress       0x3F

LiquidCrystal_I2C lcd(lcdAdress,lcdHorizontal,lcdVertical);

void Lcd_init()
{
    lcd.init(); 
    lcd.backlight();
}

void Lcd_gotoxy(int x,int y)
{
  lcd.setCursor(x,y);
}

void Lcd_setInt(int i,int x,int y)
{
  Lcd_gotoxy(x,y);
  lcd.print(i); 
}

void Lcd_print(String st,int x,int y)
{
  Lcd_gotoxy(x,y);
  lcd.print(st); 
}

void Lcd_clear()
{
  lcd.clear();
}

void Lcd_clearY(int y)
{
    Lcd_print("                    ",0,y);
}
