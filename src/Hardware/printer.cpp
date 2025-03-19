//
// Created by keqiu on 25-3-17.
//

#include <Arduino.h>
#include "printerSPI.h"
#include "stepmotor.h"
#include "printer.h"
#include "printerTemp.h"
#include "paperDetect.h"
#include "SPI.h"

#define STB6_Pin 32
#define STB5_Pin 33
#define STB4_Pin 25
#define STB3_Pin 26
#define STB2_Pin 27
#define STB1_Pin 14
//打印的六个通道

#define LAT_Pin 12
#define VH_EN_Pin 17

#define PRINT_TIME 1700 //打印时间选择在1.5ms至3ms即可，过长会损坏打印头。手册为300ns
#define PRINT_WAIT_TIME 200 //冷却时间200us

#define LAT_TIM 1 //锁存时间1us,手册是1ns可以行吗？


 void PrinterPowerOn()
{
    digitalWrite(VH_EN_Pin,HIGH);
}

 void PrinterPowerOff()
{
    digitalWrite(VH_EN_Pin,LOW);
}


 void PrinterPowerInit()
{
    pinMode(VH_EN_Pin,OUTPUT);

    PrinterPowerOff();//默认关闭打印头电源

}

 void StbOff()
{
    digitalWrite(STB1_Pin,LOW);
    digitalWrite(STB2_Pin,LOW);
    digitalWrite(STB3_Pin,LOW);
    digitalWrite(STB4_Pin,LOW);
    digitalWrite(STB5_Pin,LOW);
    digitalWrite(STB6_Pin,LOW);
}

 void StbInit()
{
    pinMode(STB1_Pin,OUTPUT);
    pinMode(STB2_Pin,OUTPUT);
    pinMode(STB3_Pin,OUTPUT);
    pinMode(STB4_Pin,OUTPUT);
    pinMode(STB5_Pin,OUTPUT);
    pinMode(STB6_Pin,OUTPUT);

    StbOff();//默认关闭打印通道
}

 void LatInit()
{
    pinMode(LAT_Pin,OUTPUT);
    digitalWrite(LAT_Pin,HIGH);
}

 void LatOff()
{
    digitalWrite(LAT_Pin,HIGH);
}


void PrinterInit()
{

    StepmotorInit();//步进电机初始化
    PrinterPowerInit();//打印头电源引脚初始化
    StbInit();//打印通道初始化

    LatInit();//锁存引脚初始化

    SPIInit();
}



static void StopPrinting()
{
    PrinterPowerOff();
    StbOff();

    digitalWrite(LAT_Pin,HIGH);
}


static void StbRun(uint8_t stbNum)
{
    switch (stbNum)
    {
    case 1:
        digitalWrite(STB1_Pin,HIGH);
        delayMicroseconds(PRINT_TIME);//打印时间选择在1.5ms至3ms即可，过长会损坏打印头。手册为300ns
        digitalWrite(STB1_Pin,LOW);
        delayMicroseconds(PRINT_WAIT_TIME);//冷却时间200us,0.2ms
        break;
    case 2:
        digitalWrite(STB2_Pin,HIGH);
        delayMicroseconds(PRINT_TIME);
        digitalWrite(STB2_Pin,LOW);
        delayMicroseconds(PRINT_WAIT_TIME);//冷却时间200us
        break;
    case 3:
        digitalWrite(STB3_Pin,HIGH);
        delayMicroseconds(PRINT_TIME);
        digitalWrite(STB3_Pin,LOW);
        delayMicroseconds(PRINT_WAIT_TIME);//冷却时间200us
        break;
    case 4:
        digitalWrite(STB4_Pin,HIGH);
        delayMicroseconds(PRINT_TIME);
        digitalWrite(STB4_Pin,LOW);
        delayMicroseconds(PRINT_WAIT_TIME);//冷却时间200us
        break;
    case 5:
        digitalWrite(STB5_Pin,HIGH);
        delayMicroseconds(PRINT_TIME);
        digitalWrite(STB5_Pin,LOW);
        delayMicroseconds(PRINT_WAIT_TIME);//冷却时间200us
        break;
    case 6:
        digitalWrite(STB6_Pin,HIGH);
        delayMicroseconds(PRINT_TIME);
        digitalWrite(STB6_Pin,LOW);
        delayMicroseconds(PRINT_WAIT_TIME);//冷却时间200us
        break;
    default:
        break;
    }
}

bool StbWorking(bool needStop,uint8_t stbNum)
{
    if (needStop == true)
    {
        StepmotorStop();
        StopPrinting();
        return true;
    }




    StbRun(stbNum);
    StepmotorRunStep(4);//四步为一行的长度,补齐3步刚好为1行

    return false;

}


static void SendOnelineData(uint8_t* data)
{

    SPICommand(data,48);//一个通道控制64个点,每一行有6个通道，共384个点,384bit,48byte

    digitalWrite(LAT_Pin,LOW);
    delayMicroseconds(1);//锁存保持时间
    digitalWrite(LAT_Pin,HIGH);

}

void StartPrintingByOneStb(uint8_t StbNum,uint8_t* data,uint32_t size)
{
    uint32_t printEnd = 0;
    uint8_t* pData = data;
    bool needStop = false;


    PrinterPowerOn();
    StbOff();
    LatOff();

    while (true)
    {

        if (size > printEnd)
        {

            SendOnelineData(pData);

            pData+=48;//移动至下一行的48Byte数据
            printEnd+=48;//打印结束判断
        }
        else
            needStop = true;

        if (StbWorking(needStop,StbNum))
        {
            break;
        }
    }

    StepmotorRunStep(40);//打印一次完成后让纸移动一点距离
    StepmotorStop();
}




static void setDebugData(uint8_t *print_data)
{
    for (uint32_t index = 0; index < 48 * 5; ++index)
    {
        //0X55 = 0101 0101 0为白，1为黑
        print_data[index] = 0x55;
    }
}


void PrinterTest()
{
    uint8_t print_data[48 * 5] = {0};//打印5行
    uint32_t size = 48*5;

    setDebugData(print_data);
    StartPrintingByOneStb(1,print_data,size);
    setDebugData(print_data);
    StartPrintingByOneStb(2,print_data,size);
    setDebugData(print_data);
    StartPrintingByOneStb(3,print_data,size);
    setDebugData(print_data);
    StartPrintingByOneStb(4,print_data,size);
    setDebugData(print_data);
    StartPrintingByOneStb(5,print_data,size);
    setDebugData(print_data);
    StartPrintingByOneStb(6,print_data,size);

    StepmotorRunStep(200);
}



