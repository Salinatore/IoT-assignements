#include <Arduino.h>
#include "kernel/Logger.h"
#include "kernel/MsgService.h"
#include "model/HWPlatform.h"

// #define __TESTING_HW__

bool isOn = false;
HWPlatform* pHWPlatform;

void setup() {
  MsgService.init();
  Logger.log(":::::: Sweeping System ::::::");

  pHWPlatform = new HWPlatform();
  pHWPlatform->init();
  
#ifndef __TESTING_HW__

#endif

#ifdef __TESTING_HW__

#endif
}

void loop() {
    if (MsgService.isMsgAvailable()) {
      Msg* msg = MsgService.receiveMsg();
      delete msg;
      Led* pLed = pHWPlatform->getLed();
      if (!isOn) {
        isOn = true;
        pLed->switchOn();
      } else {
        isOn = false;
        pLed->switchOff();
      }
    }
}
