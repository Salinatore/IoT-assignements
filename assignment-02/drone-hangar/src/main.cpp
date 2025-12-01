#include <Arduino.h>
#include "config.h"
#include "kernel/Scheduler.h"
#include "kernel/Logger.h"
#include "kernel/MsgService.h"
#include "model/HWPlatform.h"
#include "tasks/TestHWTask.h"
#include "tasks/DroneTask.h"
#include "tasks/AlarmTask.h"
#include "tasks/LedTask.h"

// #define __TESTING_HW__

Scheduler sched;

HWPlatform* pHWPlatform;
Context* pContext;

void setup() {
  MsgService.init();
  sched.init(50);
  Logger.log(":::::: Drone System ::::::");

  pHWPlatform = new HWPlatform();

#ifndef __TESTING_HW__
  pContext = new Context();

  Task* pDroneTask = new DroneTask(
      pContext, 
      pHWPlatform->getDPD(),
      pHWPlatform->getDDD(), 
      pHWPlatform->getMotor()
  );
  pDroneTask->init(50);
  pHWPlatform->getMotor()->on();
  sched.addTask(pDroneTask);

  Task* pAlarmTask = new AlarmTask(
      pContext, 
      pHWPlatform->getTempSensor(),
      pHWPlatform->getLCD(),
      pHWPlatform->getButton()
  );
  pAlarmTask->init(50);
  sched.addTask(pAlarmTask);

  Task* pLedTask = new LedTask(
      pContext, 
      pHWPlatform->getLed1(),
      pHWPlatform->getLed2(),
      pHWPlatform->getLedR()
  );
  pLedTask->init(50);
  sched.addTask(pLedTask);
#endif

#ifdef __TESTING_HW__

#endif
}

void loop() {
    sched.schedule();
}
