#include "MyLcd.h"
#include "Arduino.h"
#include <LiquidCrystal_I2C.h>

#define I2C_ADDR    0x27
#define LCD_COLUMNS 16
#define LCD_LINES   2

LiquidCrystal_I2C lcd(I2C_ADDR, LCD_COLUMNS, LCD_LINES);

MyLcd::MyLcd(){
    lcd.init();
    lcd.backlight();
    lcd.clear();
    lcd.setCursor(0, 0);
}

void MyLcd::writeMessage(String message){
    lcd.clear();
    lcd.setCursor(0, 0); 
    lcd.print(message);
    Serial.println(message);
};