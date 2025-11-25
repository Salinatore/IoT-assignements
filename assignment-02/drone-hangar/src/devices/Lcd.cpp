#include "Lcd.h"
#include "Arduino.h"
#include <LiquidCrystal_I2C.h>
#include <string.h>

Lcd::Lcd(){
    LiquidCrystal_I2C lcd(I2C_ADDR, LCD_COLUMNS, LCD_LINES);
    lcd.init();
    lcd.backlight();
    lcd.clear();
    lcd.setCursor(0, 0);
}

void Lcd::writeMessage(string message){
    int so 
    lcd.clear();
    lcd.setCursor(0, 0); 
    lcd.print(message);
    Serial.println(message);
};