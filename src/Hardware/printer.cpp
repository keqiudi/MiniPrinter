//
// Created by keqiu on 25-3-17.
//
#include "printerSPI.h"
#include "printerConfig.h"
#include "printer.h"
#include "stepmotor.h"
#include "BLE.h"
#include <device.h>
#include "SPI.h"
#include "utils/myQueue.h"


float addTime[6] = {0};//6个通道分别的补偿加热时间


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
    PrinterPowerInit();//打印头电源引脚初始化
    StepmotorInit();//步进电机初始化
    StbInit();//打印通道初始化
    LatInit();//锁存引脚初始化
    SPIInit();
}

void ClearAddTime()
{
    memset(addTime,0,sizeof(addTime));
}

void CalculateAddTime(uint8_t* data)
{
    float tmpAddTime = 0;
    ClearAddTime();

    for (int i = 0;i<6;i++)
    {
        for (int j = 0;j<8;j++)
        {
            addTime[i] += data[i*8+j];  //一个Byte代表8个点,8个点都选中就是 0xFF 255,一个通道控制8个Byte 255*8 = 2040
        }
        tmpAddTime = addTime[i]*addTime[i]; //max: 2040*2040 = 4161600
        addTime[i] = kAddTime * tmpAddTime;//max：0.001*4161600 = 4161.6
    }
}

//检查异常情况同时上报
bool printerErrorCheck(bool needReport)
{

    DeviceStatus* pDevice = getDeviceStatus();

    //缺纸异常,上报
    if (pDevice->paperStatus == PAPER_LACK)
    {
        Serial.printf("status:%d\r\n",pDevice->paperStatus);
        if (needReport)
        {
            ClearQueueBuffer();
            BLEReport();
        }
        Serial.printf("产生异常：缺纸！");
        return true;
    }

    //温度过高异常,上报
    if (pDevice->temperature >= 50)//手册工作温度为50度
    {
        if (needReport)
        {
            ClearQueueBuffer();
            BLEReport();
        }
        Serial.printf("产生异常：打印机温度过高");
        return true;
    }

    return false;
}


static void StopPrinting()
{
    PrinterPowerOff();//打印头电源关闭
    StbOff();//通道电平复位

    digitalWrite(LAT_Pin,HIGH);//LAT复位
}


static void SendOnelineData(uint8_t* data)
{
    CalculateAddTime(data);

    SPICommand(data,MAX_ONELINE_BYTE);//一个通道控制64个点,每一行有6个通道，共384个点,384bit,48byte

    digitalWrite(LAT_Pin,LOW);
    delayMicroseconds(LAT_TIME);//锁存保持时间
    digitalWrite(LAT_Pin,HIGH);

}

//通道打印
static void StbRun(uint8_t stbNum)
{
    switch (stbNum)
    {
    case 1:
        digitalWrite(STB1_Pin,HIGH);
        delayMicroseconds(PRINT_TIME+addTime[0]);//打印时间选择在1.5ms至3ms即可，过长会损坏打印头。手册为300ns
        digitalWrite(STB1_Pin,LOW);
        delayMicroseconds(PRINT_WAIT_TIME);//冷却时间200us,0.2ms
        break;
    case 2:
        digitalWrite(STB2_Pin,HIGH);
        delayMicroseconds(PRINT_TIME+addTime[1]);
        digitalWrite(STB2_Pin,LOW);
        delayMicroseconds(PRINT_WAIT_TIME);//冷却时间200us
        break;
    case 3:
        digitalWrite(STB3_Pin,HIGH);
        delayMicroseconds(PRINT_TIME+addTime[2]);
        digitalWrite(STB3_Pin,LOW);
        delayMicroseconds(PRINT_WAIT_TIME);//冷却时间200us
        break;
    case 4:
        digitalWrite(STB4_Pin,HIGH);
        delayMicroseconds(PRINT_TIME+addTime[3]);
        digitalWrite(STB4_Pin,LOW);
        delayMicroseconds(PRINT_WAIT_TIME);//冷却时间200us
        break;
    case 5:
        digitalWrite(STB5_Pin,HIGH);
        delayMicroseconds(PRINT_TIME+addTime[4]);
        digitalWrite(STB5_Pin,LOW);
        delayMicroseconds(PRINT_WAIT_TIME);//冷却时间200us
        break;
    case 6:
        digitalWrite(STB6_Pin,HIGH);
        delayMicroseconds(PRINT_TIME+addTime[5]);
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
    //所有通道打印
    if (stbNum == ALL_STB_NUM)
    {
        for (uint8_t stbIndex=1;stbIndex<7;stbIndex++)
        {
            StbRun(stbIndex);
            if (stbIndex%2 ==0)
            {
                StepmotorRunStep(1);   //优化为：加热时电机同时移动,减少卡顿和等待时间
            }
        }
        StepmotorRunStep(1);
        // StepmotorRunStep(4);//当通道加热时间变长后，等待一行的6个通道加热完成时间变长，电机才会移动,会导致打印时一卡一卡的，影响体验
    }
    else//单通道打印
    {
        StbRun(stbNum);
        StepmotorRunStep(4);//打印后应该向下移动1行的距离,4不为一行
    }

    return false;

}


//单通道数组打印
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
        {
            needStop = true;
        }

        if (StbWorking(needStop,StbNum))
        {
            break;
        }

        if (printerErrorCheck(true))
        {
            StepmotorStop();
            return;
        }
    }

    StepmotorRunStep(100);//打印一次完成后让纸移动一点距离
    StepmotorStop();

    Serial.print("print finished!\r\t");
}

//多通道数组打印
void StartPrintingByAllStb(uint8_t* data,uint32_t size)
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
         {
             needStop = true;
         }

         if (StbWorking(needStop,ALL_STB_NUM))
         {
             break;
         }

         if (printerErrorCheck(true))
         {
             StepmotorStop();
             return;
         }
     }

    StepmotorRunStep(100);//打印一次完成后让纸移动一点距离
    StepmotorStop();

    Serial.print("print finished!\r\t");
 }


//可变队列环形缓冲区打印
void StartPrintingByQueueBuffer()
{
    PrinterPowerOn();
    StbOff();//stb通道复位
    LatOff();//Lat引脚复位

    while (true)
    {
        if (getLeftLine()>0)
        {
            uint8_t* pData = readFromQueueBuffer();

            if (pData != nullptr)
            {
                SendOnelineData(pData);
            }

            StbWorking(false,ALL_STB_NUM);
        }
        else
        {
            StepmotorStop();
            StopPrinting();
            break;
        }

        if (printerErrorCheck(true))
        {
            StepmotorStop();
            StopPrinting();
            break;
        }
    }

    StepmotorRunStep(200);//打印一次完成后让纸移动一点距离,方便下次打印
    StepmotorStop();//电机复位

    Serial.print("print finished!\r\t");
}


static void setDebugData(uint8_t *print_data)
{
    for (uint32_t index = 0; index < 48 * 5; ++index)
    {
        //0X55 = 0101 0101 0为白，1为黑
        print_data[index] = 0x55;
    }
}


/*进行打印测试，所有通道分别打印5行*/
void PrinterStbTest()
{
    uint8_t print_data[48 * 5] = {0};
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
