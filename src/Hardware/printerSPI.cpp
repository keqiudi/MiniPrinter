//
// Created by keqiu on 25-3-17.
//

#include "printerSPI.h"
#include <Arduino.h>
#include <SPI.h>

#define SPI_CLK 15
#define SPI_MOSI 13

//#define SPI_MISO 16
//#define CS
//这里没有使用CS引脚和打印头的SPI_MISO引脚


SPIClass printerSPI = SPIClass(HSPI);//创建SPI对象,支持多个SPI总线

SPISettings printerSPI_Settings(1000000, MSBFIRST, SPI_MODE0);//自定义SPI通信参数



void SPIInit()
{
    pinMode(SPI_CLK,OUTPUT);
    pinMode(SPI_MOSI,OUTPUT);
    //初始化SPI通信的引脚

    //SPI.begin();不用创建SPI对象，直接启动,只适用于项目中只有一个SPI总线的情况
    printerSPI.begin(SPI_CLK, -1, SPI_MOSI,-1);//初始化SPI总线
    printerSPI.setFrequency(1000000);//设置SPI频率
}


void SPICommand(uint8_t* data,uint8_t size)
{

    printerSPI.beginTransaction(printerSPI_Settings);//开启事务,自定义SPI通信参数,适用于多个SPI设备不同频率单独控制的情况

    Serial.printf("data:%x\r\n",data[0]);

    printerSPI.transfer(data,size);//如果只有一条SPI命令，可以直接使用transfer函数，不用开启结束事务
    /*transfer函数是SPI中的双向传输，它不仅将数据发送到从设备，还会等待从设备发送回数据并将其存储在相同的 data 数组中,这里由于我们不接受打印机数据，所以会将data地址中的数据清空!!!*/

    Serial.printf("data:%x\r\n",data[0]);
    printerSPI.endTransaction();//结束事务，恢复默认参数
}





