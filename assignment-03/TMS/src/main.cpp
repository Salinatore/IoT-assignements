#include <Arduino.h>
#include "config.h"
#include "model/HWPlatform.h"
#include "tasks/TestHWTask.h"
#include "tasks/HWTask.h"

// #define __TESTING_HW__


HWPlatform *pHWPlatform;
TaskHandle_t Task1;

void Task1code(void* parameter){
  pHWPlatform = new HWPlatform();
  HWTask* hwTask = new HWTask(pHWPlatform->getSonar(), pHWPlatform->getRedLed(), pHWPlatform->getGreenLed());
  hwTask->task(parameter);
}


void setup()
{
  xTaskCreatePinnedToCore(Task1code,"hwTask",10000,NULL,1,&Task1,0);
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
