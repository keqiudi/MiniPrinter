//
// Created by keqiu on 25-3-17.
//

#include <Arduino.h>
#include <Ticker.h>

#include "./Hardware/Stepmotor.h"

#define STEPMOTOR_BP_Pin 21
#define STEPMOTOR_BM_Pin 19
#define STEPMOTOR_AP_Pin 23
#define STEPMOTOR_AM_Pin 22



#define STEPMOTOR_WAITTIME 2 //根据手册我们使用的是500Hz,我们使用每走一步时间为2ms,频率为500Hz




//步进电机每走四步,纸张行进一行
// A+ B+ A- B-
uint8_t StepmotorTimeTable[8][4] = {
    {1, 0, 0, 1},
    {0, 0, 0, 1},
    {0, 0, 1, 1},
    {0, 0, 1, 0},
    {0, 1, 1, 0},
    {0, 1, 0, 0},
    {1, 1, 0, 0},
    {1, 0, 0, 0}
};
//来自打印头步进电机时序表,每一个时序代表步进电机走一步

uint8_t motorIndex = 0;

Ticker StepmotorTimer;

void StepmotorInit()
{

    pinMode(STEPMOTOR_BP_Pin,OUTPUT);
    pinMode(STEPMOTOR_BM_Pin,OUTPUT);
    pinMode(STEPMOTOR_AP_Pin,OUTPUT);
    pinMode(STEPMOTOR_AM_Pin,OUTPUT);

    digitalWrite(STEPMOTOR_BP_Pin,LOW);
    digitalWrite(STEPMOTOR_BM_Pin,LOW);
    digitalWrite(STEPMOTOR_AP_Pin,LOW);
    digitalWrite(STEPMOTOR_AM_Pin,LOW);
}



//指定步数运行,阻塞式运行
void StepmotorRunStep(uint32_t Steps)
{
     uint32_t steps = Steps;
     while (steps)
     {
         digitalWrite(STEPMOTOR_AP_Pin, StepmotorTimeTable[motorIndex][0]);
         digitalWrite(STEPMOTOR_BP_Pin, StepmotorTimeTable[motorIndex][1]);
         digitalWrite(STEPMOTOR_AM_Pin, StepmotorTimeTable[motorIndex][2]);
         digitalWrite(STEPMOTOR_BM_Pin, StepmotorTimeTable[motorIndex][3]);

         motorIndex++;
         motorIndex%=8;//8步为一个循环,4步为一行

         steps--;

         delay(STEPMOTOR_WAITTIME);//步进电机速度控制,不控制可能会造成电机还没走完下一次就到来了
     }

}



//定时器回调函数
void TimerCallBack()
{

    digitalWrite(STEPMOTOR_AP_Pin, StepmotorTimeTable[motorIndex][0]);
    digitalWrite(STEPMOTOR_BP_Pin, StepmotorTimeTable[motorIndex][1]);
    digitalWrite(STEPMOTOR_AM_Pin, StepmotorTimeTable[motorIndex][2]);
    digitalWrite(STEPMOTOR_BM_Pin, StepmotorTimeTable[motorIndex][3]);

    motorIndex++;
    motorIndex%=8;
}



//使用定时器非阻塞运行
void StepmotorStart()
{
    if (StepmotorTimer.active() == false)
    {
        StepmotorTimer.attach_ms(STEPMOTOR_WAITTIME, TimerCallBack); //每2ms执行一次回调函数
    }

}

void StepmotorStop()
{
    digitalWrite(STEPMOTOR_BP_Pin,LOW);
    digitalWrite(STEPMOTOR_BM_Pin,LOW);
    digitalWrite(STEPMOTOR_AP_Pin,LOW);
    digitalWrite(STEPMOTOR_AM_Pin,LOW);

    motorIndex = 0;

    if (StepmotorTimer.active() == true)
    {
        StepmotorTimer.detach();
    }
}