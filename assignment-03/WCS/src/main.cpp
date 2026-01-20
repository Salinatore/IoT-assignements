#include <Arduino.h>
#include "config.h"
#include "kernel/Scheduler.h"
#include "kernel/Logger.h"
#include "kernel/MsgService.h"
#include "model/HWPlatform.h"
#include "tasks/TestHWTask.h"
#include "tasks/WCSTask.h"

// #define __TESTING_HW__

Scheduler sched;

HWPlatform *pHWPlatform;

void setup()
{
  MsgService.init();
  sched.init(75);
  Logger.log(":::::: WCS ::::::");

  pHWPlatform = new HWPlatform();

#ifndef __TESTING_HW__
  Task *pWCSTask = new WCSTask(
    pHWPlatform->getMotor(),
    pHWPlatform->getLCD(),
    pHWPlatform->getButton(),
    pHWPlatform->getPot()
  );
  pWCSTask->init(70);
  sched.addTask(pWCSTask);
  
#endif

#ifdef __TESTING_HW__
  Task *pTestHWTask = new TestHWTask(pHWPlatform);
  pTestHWTask->init(200);
  sched.addTask(pTestHWTask);
#endif
}

void loop()
{
  sched.schedule();
}
