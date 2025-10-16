#ifndef INTERRUPT_H
#define INTERRUPT_H

#include <stdint.h>

void internalEnableInterrupt(uint8_t pin, void (*userFunction)(), uint8_t mode);

void internalDisableInterrupt(uint8_t pin);

#endif