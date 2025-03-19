//
// Created by keqiu on 25-3-10.
//
#include <Arduino.h>
#include <../key.h>
#include <../led.h>
#include "../printer.h"
#include "../stepmotor.h"

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
            if (millis() - KeyPressTick > LONG_PRESS_TIME)
            {
                StepmotorRunStep(200);
            }
            else
            {
                 PrinterTest();

            }
            KeyPressed = false;
        }

    }

}
