//
// Created by keqiu on 25-3-11.
//


#include <Arduino.h>
#include "battery.h"

#include <device.h>

#include "esp_adc_cal.h"

#define Battery_ADC_Pin 34
#define DEFAULT_VREF   1100            // 默认基准电压 (mV)

#define MAX_BUFFER_SIZE 100

//单片机ADC采集范围为0~4095，对应电压范围为0~3.3V

//而我们的电池电压范围为3.7V~4.2V,超过采集范围，所以我们选择分压采集


uint16_t voltageBuffer [100] = {0};
uint8_t bufferIndex = 0;
uint8_t volCounter = 0;

 uint16_t getVoltage()
{

    uint16_t ADValue =  0;
    uint16_t voltage = 0;

    voltage = analogReadMilliVolts(Battery_ADC_Pin);//获取电池电压值,该函数内部进行了校准,单位为mV

     // ADValue = analogRead(Battery_ADC_Pin);
     // voltage = (ADValue / 4095)*3.3;
    /*直接使用该函数获取AD值转换好像误差太大了*/

    return voltage;//得到分压的电阻电压
}

/**写的校准函数，实际使用和直接调用analogReadMilliVolts好像精度差不多**/

 uint32_t getCalibrationVoltage()
{
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_11db);

    esp_adc_cal_characteristics_t adc_chars;
    esp_err_t cali = esp_adc_cal_characterize(
    ADC_UNIT_1, ADC_ATTEN_11db, ADC_WIDTH_BIT_12, DEFAULT_VREF, &adc_chars);

    uint32_t rawData = adc1_get_raw(ADC1_CHANNEL_6);
    uint32_t voltage = esp_adc_cal_raw_to_voltage(rawData, &adc_chars);

    return voltage;
}

/*由于打印时会出现电量大幅度波动的情况，所以这里采用均值滤波*/
uint8_t DoAverageFilter()
{
     uint16_t filteredVoltage = 0;
     uint8_t battery = 0 ;
     uint32_t sum = 0;
     voltageBuffer[bufferIndex] = getVoltage()*2;
     if (volCounter<MAX_BUFFER_SIZE)
     {
         volCounter++;
     }
     bufferIndex = (bufferIndex + 1) % MAX_BUFFER_SIZE;

     for (int i=0;i<MAX_BUFFER_SIZE;i++)
     {
         sum += voltageBuffer[i];
     }

     filteredVoltage = sum / volCounter;

     battery = map(filteredVoltage,3700,4200,0,100);

     return battery;
}



void BatteryDetect()
{

    uint8_t Battery = 0;


    // Battery = map(getVoltage()*2,3700,4200, 0, 100);//电压*2才是VBAT的电压,3700~4200对应3.7V~4.2V扩大100倍
     Battery = DoAverageFilter();

    if (Battery > 100)
    {
        Battery = 100;
    }

     getDeviceStatus()->battery = Battery;

    Serial.printf("Battery:%d%%\r\n",Battery);
}



