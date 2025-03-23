//
// Created by keqiu on 25-3-19.
//

#ifndef DEVICE_H
#define DEVICE_H

#include <Arduino.h>



typedef enum
{
    PRINTER_INIT,//初始化完成
    PRINTER_START,//开始打印
    PRINTER_WORKING,//打印中
    PRINTER_IDLE,//空闲
}PrinterStatus;


typedef enum
{
    PAPER_FULL,
    PAPER_LACK,
}PaperStatus;


typedef struct
{
    uint8_t battery;
    uint8_t temperature;
    PaperStatus paperStatus;
    PrinterStatus printerStatus;
    bool bleReadFlag;
}DeviceStatus;

void deviceStatusInit();

DeviceStatus* getDeviceStatus();

void setPaperStatus(PaperStatus paperStatus);

void setBleReadFlag(bool status);

#endif //DEVICE_H
