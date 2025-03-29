//
// Created by keqiu on 25-3-10.
//
#include <Arduino.h>
#include <./Hardware/key.h>
#include <./Hardware/led.h>
#include "./Hardware/printer.h"
#include "./Hardware/stepmotor.h"
#include "device.h"

#define LONG_PRESS_TIME 1000

bool KeyPressed = false;
bool LongPressed = false;
uint32_t KeyPressTick = 0;

bool LED_Status = false;

void KeyInit()
{
    pinMode(KEY_Pin,INPUT);
}


void KeyScan()
{

    DeviceStatus* pDevice = getDeviceStatus();

    if (digitalRead(KEY_Pin) == LOW)
    {
        delay(10);
        if (digitalRead(KEY_Pin) == LOW)
        {
            if (KeyPressed == false)
            {
                KeyPressed = true;
                KeyPressTick = millis();
            }

        }
    }

    if (digitalRead(KEY_Pin) == HIGH)
    {

        if (KeyPressed == true)
        {
            if (pDevice->paperStatus == PAPER_FULL && pDevice->printerStatus == PRINTER_IDLE)
            {
                if (millis() - KeyPressTick > LONG_PRESS_TIME)//长按操作
                {

                    StepmotorRunStep(200);

                }
                else//短按操作
                {
                    PrinterStbTest();
                }
            }
            else
            {
                Serial.printf("正在打印中... 禁止使用按键调试");
            }
            KeyPressed = false;
        }

    }

}
