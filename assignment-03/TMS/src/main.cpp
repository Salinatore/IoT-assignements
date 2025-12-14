#include <Arduino.h>
#include "config.h"
#include "kernel/Scheduler.h"
#include "model/HWPlatform.h"
#include "tasks/TestHWTask.h"
#include "tasks/WCSTask.h"

// #define __TESTING_HW__

Scheduler sched;

HWPlatform *pHWPlatform;

void setup()
{


  pHWPlatform = new HWPlatform();

#ifndef __TESTING_HW__
  
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
