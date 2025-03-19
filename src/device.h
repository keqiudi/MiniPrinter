//
// Created by keqiu on 25-3-19.
//

#ifndef DEVICE_H
#define DEVICE_H

#include <Arduino.h>



typedef enum
{
    PRINTER_INIT,
    PRINTER_START,
    PRINTER_WORKING,
    PRINTER_STOP,
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
}DeviceStatus;

void deviceStatusInit();

DeviceStatus* getDeviceStatus();

void setPaperStatus(PaperStatus paperStatus);


#endif //DEVICE_H
