//
// Created by keqiu on 25-3-18.
//


#include "task.h"
#include <Arduino.h>
#include <device.h>

#include "Hardware/PaperDetect.h"
#include "Hardware/BLE.h"
#include "Hardware/printer.h"
#include "Hardware/key.h"
#include "Hardware/led.h"
#include "Hardware/battery.h"
#include "Hardware/printerTemp.h"


void StartKeyTask(void *pvParameters)
{
    KeyInit();
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
        vTaskDelay(500);
    }
}

void StartDeviceStatusTask(void *pvParameters)
{
    TempDetectInit();//打印头温度检测初始化
    PaperDetectInit();//缺纸检测初始化

    for (;;)
    {
        TempDetect();
        PaperDetect();
        BatteryDetect();
    }
}



void PrinterRunning()
{
    DeviceStatus* pDevice = getDeviceStatus();

    if (pDevice->bleReadFlag == true)
    {
        if (pDevice->printerStatus == PRINTER_IDLE || pDevice->printerStatus == PRINTER_INIT)
        {
            pDevice->bleReadFlag = false;
            pDevice->printerStatus = PRINTER_START;
            BLEReport();//打印开始上报一下
            LedControl(LED_SLOW_BLINK);
            Serial.printf("");
        }
    }



    //开始打印
    if (pDevice->printerStatus == PRINTER_START)
    {
        pDevice->printerStatus = PRINTER_WORKING;
        //打印中
        StartPrintingByAllStb();

        pDevice->printerStatus = PRINTER_IDLE;
    }
}


void TaskInit()
{
    Serial.begin(115200);

    LedInit();
    PrinterPowerInit();//打印头电源引脚初始化
    PrinterInit();

    /*1.任务函数 2.任务名称 3.任务堆栈大小 4.任务参数 5.任务优先级 6.任务句柄*/
    xTaskCreate(StartBleTask,"BleTask",1024*10,NULL,0,NULL);
    xTaskCreate(StartKeyTask,"KeyTask",1024*5,NULL,1,NULL);
    xTaskCreate(StartDeviceStatusTask,"DeviceStatusTask",1024*5,NULL,2,NULL);
}
