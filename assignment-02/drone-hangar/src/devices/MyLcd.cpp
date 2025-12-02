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

void MyLcd::writeStateMessage(String message){
    this->stateMessage = message;
    this->refresh();
};

void MyLcd::writeAlarmMessage(String message){
    this->alarmMessage = message;
    this->refresh();
};

void MyLcd::refresh(){
    lcd.clear();
    lcd.setCursor(0, 0); 
    lcd.print(this->alarmMessage);
    lcd.setCursor(0, 1); 
    lcd.print("STATE: "+ this->stateMessage);
    lcd.flush();
}