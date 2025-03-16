#include <Arduino.h>

#include "LED.h"
#include "Key.h"
#include "Battery.h"
#include "BLE.h"
void setup()
{
    Serial.begin(115200);
    Serial.printf("serial start");

    ledInit();
    keyInit();
    BLEInit();

}

void loop()
{
    // keyScan();
    // BatteryDetect();

    BLEReport();
}





