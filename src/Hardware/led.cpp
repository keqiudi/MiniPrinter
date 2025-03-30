//
// Created by keqiu on 25-3-10.
//

#include "led.h"
#include "Arduino.h"

void LedBlink(uint32_t ms)
{
    digitalWrite(LED_Pin,LOW);
    delay(ms);
    digitalWrite(LED_Pin,HIGH);
    delay(ms);
    digitalWrite(LED_Pin,LOW);
    delay(ms);
    digitalWrite(LED_Pin,HIGH);
    delay(ms);
}

void LedControl(LED_STATUS status)
{
    switch (status)
    {
    case LED_OFF:
        digitalWrite(LED_Pin, HIGH);
        break;

    case LED_ON:  //待机状态
        digitalWrite(LED_Pin,LOW);
        break;

    case LED_FAST_BLINK:  //故障状态
        LedBlink(100);
        break;

    case LED_SLOW_BLINK: //工作状态
        LedBlink(1000);
        break;
    }
}

void LedInit()
{
  	pinMode(LED_Pin, OUTPUT);
}
