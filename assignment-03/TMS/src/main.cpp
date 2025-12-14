#include <Arduino.h>
#include "config.h"
#include "model/HWPlatform.h"
#include "tasks/TestHWTask.h"
#include "tasks/WCSTask.h"

// #define __TESTING_HW__


HWPlatform *pHWPlatform;

void setup()
{
  pHWPlatform = new HWPlatform();
  WCSTask* mainTask = new WCSTask(pHWPlatform->getSonar());
  xTaskCreatePinnedToCore(mainTask->task(),"Task1",10000,NULL,1,&Task1,0);
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

}
