#include <Arduino.h>
#include "kernel/Logger.h"
#include "kernel/MsgService.h"

// #define __TESTING_HW__

void setup() {
  MsgService.init();
  Logger.log(":::::: Sweeping System ::::::");
  
#ifndef __TESTING_HW__

#endif

#ifdef __TESTING_HW__

#endif
}

void loop() {
}