#include "lcd_control.h"
#include "setup.h"
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(I2C_ADDR, LCD_COLUMNS, LCD_LINES);

void setUpLCD() {
  lcd.init();          
  lcd.backlight();     
  lcd.clear();         
  lcd.setCursor(0, 0); 
}

void writeGo() {
  lcd.clear();         
  lcd.setCursor(0, 0); 
  lcd.print("GO");
}

void writeWelcomeMessage() {
  lcd.clear();         
  lcd.setCursor(0, 0);
  lcd.print("Welcome to TOS!");
  lcd.setCursor(0, 1);
  lcd.print("Press B1 to Start");
}

void writeSequence(int seq) {
  lcd.clear();         
  lcd.setCursor(0, 0); 
  lcd.print(String(seq));
}

void writeWinMessage(int score) {
  lcd.clear();         
  lcd.setCursor(0, 0); 
  lcd.print("Score: " + String(score) + " WIN");
}

void writeLoseMessage(int score) {
  lcd.clear();         
  lcd.setCursor(0, 0); 
  lcd.print("Score: " + String(score) + " LOSE");
}