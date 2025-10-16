#include "interrupt.h"
#include <EnableInterrupt.h>

void internalEnableInterrupt(uint8_t pin, void (*userFunction)(), uint8_t mode) {
  enableInterrupt(pin, userFunction, mode);
}

void internalDisableInterrupt(uint8_t pin) {
  disableInterrupt(pin); 
}