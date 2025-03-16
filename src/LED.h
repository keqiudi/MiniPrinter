//
// Created by keqiu on 25-3-10.
//

#ifndef LED_H
#define LED_H

#define LED_Pin 18

typedef enum
{
    LED_OFF,
    LED_ON,
    LED_FAST_BLINK,
    LED_SLOW_BLINK,
} LED_STATUS;


void ledControl(LED_STATUS status);

void ledInit();

#endif //LED_H
