#include "Lcd.h"
#include "Arduino.h"
#include <LiquidCrystal_I2C.h>

#define I2C_ADDR    0x27
#define LCD_COLUMNS 16
#define LCD_LINES   2

LiquidCrystal_I2C lcd(I2C_ADDR, LCD_COLUMNS, LCD_LINES);

Lcd::Lcd(){
    lcd.init();
    lcd.backlight();
    lcd.clear();
    lcd.setCursor(0, 0);
}

void Lcd::writeModeMessage(String message){
    this->modeMessage = message;
    this->refresh();
};

void Lcd::writePercMessage(String message){
    this->percMessage = message;
    this->refresh();
};

void Lcd::refresh(){
    lcd.clear();
    lcd.setCursor(0, 0); 
    lcd.print(this->modeMessage);
    lcd.setCursor(0, 1); 
    lcd.print("VALVE: "+ this->percMessage +"%");
    lcd.flush();
}