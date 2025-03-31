//
// Created by keqiu on 25-3-18.
//


#include "task.h"
#include <Arduino.h>
#include <device.h>
#include "utils/myQueue.h"
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
        vTaskDelay(10);//慢一点执行,避免其他任务无法抢到时间片
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
        vTaskDelay(2000);
    }
}

void StartLedTask(void *pvParameters)
{
    DeviceStatus* pDevice = getDeviceStatus();
    LedInit();

    for (;;)
    {

        if (pDevice->printerStatus == PRINTER_IDLE || pDevice->printerStatus == PRINTER_INIT)
        {
            if (getBleConnect() == true)
            {
                if (pDevice->temperature > 50 || pDevice->paperStatus == PAPER_LACK)
                {
                    LedControl(LED_FAST_BLINK);
                }
                else
                {
                    LedControl(LED_ON);
                }
            }
            else
            {
                LedControl(LED_OFF);
            }

        }

        else if (pDevice->printerStatus  == PRINTER_START || pDevice->printerStatus == PRINTER_WORKING)
        {

            if (pDevice->temperature > 50 || pDevice->paperStatus == PAPER_LACK)
            {
                LedControl(LED_FAST_BLINK);
            }
            else
            {
                LedControl(LED_SLOW_BLINK);
            }
        }
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
            Serial.printf("Start Printing...");
        }
    }

    
    //开始打印
    if (pDevice->printerStatus == PRINTER_START)
    {
        pDevice->printerStatus = PRINTER_WORKING;
        //打印中
        // ArrayBuffer* pArrayBuffer = getPrinterArrayBuffer();
        // StartPrintingByAllStb(pArrayBuffer->arrayBuffer,pArrayBuffer->bufferIndex);
        StartPrintingByQueueBuffer();
        pDevice->printerStatus = PRINTER_IDLE;
    }
}


void TaskInit()
{
    Serial.begin(115200);

    PrinterInit();
    deviceStatusInit();
    QueueInit();

    /*1.任务函数 2.任务名称 3.任务堆栈大小 4.任务参数 5.任务优先级 6.任务句柄*/
    xTaskCreate(StartLedTask,"LedTask",1024*5,NULL,0,NULL);
    xTaskCreate(StartBleTask,"BleTask",1024*10,NULL,1,NULL);
    xTaskCreate(StartKeyTask,"KeyTask",1024*5,NULL,2,NULL);
    xTaskCreate(StartDeviceStatusTask,"DeviceStatusTask",1024*5,NULL,3,NULL);
}
