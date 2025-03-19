#include <Arduino.h>
#include "./Hardware/BLE.h"
#include "./Hardware/key.h"
#include "./Hardware/LED.h"
#include "./Hardware/printer.h"
#include "task.h"
#include "./Hardware/battery.h"
#include "./Hardware/printerTemp.h"
#include "./Hardware/paperDetect.h"



void setup()
{
    Serial.begin(115200);

    KeyInit();
    LedInit();
    BLEInit();

    TempDetectInit();//打印头温度检测初始化
    PaperDetectInit();//缺纸检测初始化
    PrinterPowerOff();
    PrinterInit();

}

void loop()
{
    KeyScan();
    // BatteryDetect();
    // TempDetect();
    // PaperDetect();
    // delay(1000);
}






