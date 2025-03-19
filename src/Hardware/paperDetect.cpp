//
// Created by keqiu on 25-3-16.
//

#include <Arduino.h>
#include "PaperDetect.h"
#include "device.h"

#define PaperDetect_Pin 35


volatile PaperStatus state = PAPER_FULL;

//中断服务函数
void PaperDetect_ISR()
{
    state = PAPER_LACK;//触发上升沿后，纸张缺纸
}



void PaperDetectInit()
{

    pinMode(PaperDetect_Pin,INPUT);
    attachInterrupt(digitalPinToInterrupt(PaperDetect_Pin), PaperDetect_ISR, RISING);
}


void PaperDetect()
{
    if (digitalRead(PaperDetect_Pin) == HIGH)
    {
        Serial.printf("Paper Lack\r\n");
    }
    else
    {
        state = PAPER_FULL;
    }
}





