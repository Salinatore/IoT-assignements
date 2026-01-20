#include <Arduino.h>
#include "config.h"
#include "model/HWPlatform.h"
#include "tasks/HWTask.h"
#include "kernel/Task.h"


HWPlatform *pHWPlatform;
TaskHandle_t Task1;

void Task1code(void* parameter){
  pHWPlatform = new HWPlatform();
  HWTask* hwTask = new HWTask(pHWPlatform->getSonar(), pHWPlatform->getRedLed(), pHWPlatform->getGreenLed());
  hwTask->task();
}

void setup()
{
  Serial.begin(9600);
  Serial.println("set up serial");
  //Task1code was run on core 0 whith name hwTask, 10000 byte of stack size, passing null, giving hight priority 1, null as task handler
  xTaskCreatePinnedToCore(Task1code,"hwTask",10000,NULL,1, NULL ,0);
}

void loop()
{

}
