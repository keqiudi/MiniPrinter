//
// Created by keqiu on 25-3-16.
//

#include <Arduino.h>
#include "PaperDetect.h"


#define PaperDetect_Pin 35

typedef enum
{
    PAPER_FULL,
    PAPER_LACK,

}PaperStatus;

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
        Serial.printf("Paper Lack");
    }
    else
    {
        state = PAPER_FULL;
    }
}





