//
// Created by keqiu on 25-3-19.
//


#include "device.h"
#include <Arduino.h>


DeviceStatus deviceStatus;



void deviceStatusInit()
{
    memset(&deviceStatus,0,sizeof(DeviceStatus));
    deviceStatus.battery = 100;
    deviceStatus.temperature = 25;
    deviceStatus.paperStatus = PAPER_FULL;
    deviceStatus.printerStatus = PRINTER_INIT;
    deviceStatus.bleReadFlag = false;
}

void setBleReadFlag(bool status)
{
    deviceStatus.bleReadFlag = status;
}


DeviceStatus* getDeviceStatus()
{
    return &deviceStatus;
}


void setPaperStatus(PaperStatus paperStatus)
{
    deviceStatus.paperStatus = paperStatus;
}






