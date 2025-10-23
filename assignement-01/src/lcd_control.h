#ifndef LCD_CONTROL_H
#define LCD_CONTROL_H

void setUpLCD();

void writeGo();

void writeWelcomeMessage();

void writeSequence(int seq[]);

void writeWinMessage(int score);

void writeLoseMessage(int score);

void sleepLCD();

void wakeUpLCD();

#endif