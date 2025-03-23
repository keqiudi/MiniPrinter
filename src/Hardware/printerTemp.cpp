//
// Created by keqiu on 25-3-17.
//

#include "printerTemp.h"

#include <Arduino.h>

#include "led.h"

#define TEMP_Pin 36

#define SET_ADC_BIT 12


/** 可以使用厂商提供的查表法和或公式法
  * 查表法：速度快，空间换时间
  * 公式法：效率比较低
  *
  */

float getResistanceValue()
{
    float Rx = 0;
    float R1 = 10000;//串联分压电阻值

    // uint16_t ADValue = analogRead(TEMP_Pin);AD值精度不高

    float voltage = analogReadMilliVolts(TEMP_Pin)/1000.0;//直接读取电压单位mV

    Rx = R1*(voltage/(3.3-voltage));    // 分压公式：voltage / 3.3V = Rx*(R1+Rx),变换得到该公式


    return Rx;

}

float calculateTemp()
{
    float temp = 0.0;
    float B = 3950;//热敏电阻材料常数，单位K
    float R25 = 30000;
    float T25 = 25+273.15;
    float Rx = getResistanceValue();

    //手册上给出的计算公式 Rx = R25*exp(B*(1/T-1/T25))
    // C中log就是以e为底数的
    // lnRx = lnR25 + B*(1/T-1/T25)
    // (lnRX/R25)/B = 1/T - 1/T25
    // 1/T = 1/T25 +(lnRx/R25)/B
    // T = 1/(1/T25 + (lnRx/R25)/B)
    temp = 1/(1/T25 + log(Rx/R25)/B) - 273.15;//得到的温度单位是开尔文需要转换

    return temp;
}

void TempDetectInit()
{
    analogReadResolution(SET_ADC_BIT);//设置ADC分辨率
}


void TempDetect()
{

    float temp = calculateTemp();
    Serial.printf("打印头当前温度为：%.2f\r\n",temp);

    if (temp>40)
    {
        LedControl(LED_FAST_BLINK);
    }
}