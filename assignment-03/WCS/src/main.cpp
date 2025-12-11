#include <Arduino.h>
#include "config.h"
#include "kernel/Scheduler.h"
#include "kernel/Logger.h"
#include "kernel/MsgService.h"
#include "model/HWPlatform.h"
#include "tasks/TestHWTask.h"
#include "tasks/AlarmTask.h"

// #define __TESTING_HW__

Scheduler sched;

HWPlatform *pHWPlatform;
Context *pContext;

void setup()
{
  MsgService.init();
  sched.init(50);
  Logger.log(":::::: Drone System ::::::");

  pHWPlatform = new HWPlatform();

#ifndef __TESTING_HW__
  pContext = new Context();
  
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
