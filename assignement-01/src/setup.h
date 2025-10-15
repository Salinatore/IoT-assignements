#ifndef SETUP_H
#define SETUP_H

#define BUTTON1_PIN 2
#define BUTTON2_PIN 3
#define BUTTON3_PIN 4
#define BUTTON4_PIN 5

#define LED1_PIN 10
#define LED2_PIN 11
#define LED3_PIN 12
#define LED4_PIN 13
#define LED_RED_PIN 9

//#define I2C_ADDR    0x27
//#define LCD_COLUMNS 16
//#define LCD_LINES   2

void setUpLeds();

void setUpButtons();

//void setUpLCD();

#endif
