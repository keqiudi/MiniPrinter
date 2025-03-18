#include <Arduino.h>

#include "LED.h"
#include "Key.h"
#include "Battery.h"
#include "BLE.h"
#include "paperDetect.h"
#include "TempDetect.h"
#include "stepmotor.h"
#include "printer.h"

void setup()
{
    Serial.begin(115200);
    Serial.printf("serial start");

    ledInit();
    PrinterInit();
    keyInit();
    // BLEInit();

    // PaperDetectInit();
    // TempDetectInit();
    StepmotorInit();

}

void loop()
{
    keyScan();
    // BatteryDetect();
    // BLEReport();
    // PaperDetect();
    // TempDetect();
    // delay(1000);


}





