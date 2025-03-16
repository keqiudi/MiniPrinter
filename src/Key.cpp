//
// Created by keqiu on 25-3-10.
//
#include <Arduino.h>
#include <Key.h>
#include <LED.h>
#define LONG_PRESS_TIME 1000

bool KeyPressed = false;
bool LongPressed = false;
uint32_t KeyPressTick = 0;

 bool LED_Status = false;

void keyInit()
{
    pinMode(KEY_Pin,INPUT);
}


void keyScan()
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

                    Serial.printf("long press");
                    LED_Status = !LED_Status;
                    digitalWrite(LED_Pin,LED_Status);
            }
            else
            {
                Serial.printf("short press");
                LED_Status = !LED_Status;
                digitalWrite(LED_Pin,LED_Status);
            }
            KeyPressed = false;
        }

    }

    // if (KeyPressed == false)
    //
    //     if(digitalRead(KEY_Pin) == LOW)
    //     {
    //         delay(10);
    //         if (digitalRead(KEY_Pin) == LOW)
    //         {
    //             KeyPressed = true;
    //             KeyPressTick = millis();
    //         }
    //     }
    //
    // if (KeyPressed == true)
    // {
    //
    //
    //     if (digitalRead(KEY_Pin) == HIGH)
    //     {
    //         if (millis() - KeyPressTick > LONG_PRESS_TIME)
    //         {
    //             Serial.printf("long press free");
    //             LongPressed = false;
    //
    //         }
    //         else
    //         {
    //             Serial.printf("short press");
    //         }
    //     }
    //     else
    //     {
    //         if (millis() - KeyPressTick > LONG_PRESS_TIME)
    //         {
    //             if (LongPressed == false)
    //             {
    //                 Serial.printf("long press");
    //                 LongPressed = true;
    //             }
    //         }
    //     }
    // }
}
