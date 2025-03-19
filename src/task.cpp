//
// Created by keqiu on 25-3-18.
//


#include "task.h"
#include <Arduino.h>

#include "BLE.h"
#include "printer.h"
#include "key.h"
#include "led.h"
#include "battery.h"
#include "printerTemp.h"


void StartKeyTask(void *pvParameters)
{

    for (;;)
    {
        KeyScan();
    }
}


void StartBleTask(void *pvParameters)
{
    BLEInit();
    for (;;)
    {
        BLEReport();
        vTaskDelay(5000);
    }
}



void TaskInit()
{

    /*1.任务函数 2.任务名称 3.任务堆栈大小 4.任务参数 5.任务优先级 6.任务句柄*/

    xTaskCreate(StartKeyTask,"KeyTask",1024*3,NULL,3,NULL);

    xTaskCreate(StartBleTask,"BleTask",1024*5,NULL,2,NULL);
}
