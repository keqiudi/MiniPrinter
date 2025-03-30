//
// Created by keqiu on 25-3-11.
//


#include <Arduino.h>
#include "battery.h"
#include "esp_adc_cal.h"

#define Battery_ADC_Pin 34
#define DEFAULT_VREF   1100            // 默认基准电压 (mV)


//单片机ADC采集范围为0~4095，对应电压范围为0~3.3V

//而我们的电池电压范围为3.7V~4.2V,超过采集范围，所以我们选择分压采集



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
    ADC_UNIT_1, ADC_ATTEN_11db, ADC_WIDTH_BIT_12, DEFAULT_VREF, &adc_chars
);

    uint32_t rawData = adc1_get_raw(ADC1_CHANNEL_6);
    uint32_t voltage = esp_adc_cal_raw_to_voltage(rawData, &adc_chars);

    return voltage;
}


void BatteryDetect()
{
    uint8_t Battery = 0;

    Battery = map(getVoltage()*2,3700,4200, 0, 100);//电压*2才是VBAT的电压,3700~4200对应3.7V~4.2V扩大100倍

    if (Battery > 100)
    {
        Battery = 100;
    }

    Serial.printf("Battery:%d%%\r\n",Battery);
}



