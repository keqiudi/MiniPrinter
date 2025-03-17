//
// Created by keqiu on 25-3-16.
//

#include <Arduino.h>
#include "PaperDetect.h"

#define VH_EN_Pin 17
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
    pinMode(VH_EN_Pin,OUTPUT);
    digitalWrite(VH_EN_Pin,LOW);//VH_EN拉低,MOS管断开，为进行打印时必须关闭打印头电源

    pinMode(PaperDetect_Pin,INPUT);
    attachInterrupt(digitalPinToInterrupt(PaperDetect_Pin), PaperDetect_ISR, RISING);
}


void PaperDetect()
{

    if (state == PAPER_LACK)
    {
        Serial.printf("Paper Lack!!!\n");

        if (digitalRead(PaperDetect_Pin) == HIGH)
        {
            state = PAPER_FULL;
        }
    }
}





