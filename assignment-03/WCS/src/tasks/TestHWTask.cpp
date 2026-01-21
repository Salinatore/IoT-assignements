#include "tasks/TestHWTask.h"
#include "config.h"
#include <Arduino.h>


TestHWTask::TestHWTask(HWPlatform* pHW): pHW(pHW){
}
  
void TestHWTask::tick(){
    pHW->test();
}


