//
// Created by keqiu on 25-3-10.
//

#include "LED.h"
#include "Arduino.h"

void ledBlink(uint32_t ms)
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

void ledControl(LED_STATUS status)
{
    switch (status)
    {
    case LED_OFF:
        digitalWrite(LED_Pin, HIGH);
        break;

    case LED_ON:
        digitalWrite(LED_Pin,LOW);
        break;

    case LED_FAST_BLINK:
        ledBlink(200);
        break;

    case LED_SLOW_BLINK:
        ledBlink(1000);
        break;
    }
}

void ledInit()
{
  	pinMode(LED_Pin, OUTPUT);
}
