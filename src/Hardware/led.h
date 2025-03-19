//
// Created by keqiu on 25-3-10.
//

#ifndef LED_H
#define LED_H

#define LED_Pin 18

typedef enum
{
    LED_OFF,//待机
    LED_ON,//蓝牙连接
    LED_FAST_BLINK,//运行异常
    LED_SLOW_BLINK,//打印中
} LED_STATUS;


void LedControl(LED_STATUS status);

void LedInit();

#endif //LED_H
