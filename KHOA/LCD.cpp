#include "LCD.h"

#define lcdVertical     2
#define lcdHorizontal   16

LiquidCrystal_I2C lcd(0x27, lcdHorizontal, lcdVertical);

void Lcd_init()
{
  Wire.begin(D4, D3);           //Bắt đầu 2 chân SDA và SCK của I2C
  // Khởi tạo LCD
  lcd.init();                 //Bắt đầu màn hình
  lcd.backlight();            // Bật đèn nền
  lcd.home();                 //Đưa con trỏ về vị trí 0,0
  lcd.display();              // Hiển thị lên màn hình.
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
