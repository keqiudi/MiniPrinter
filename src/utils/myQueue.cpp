//
// Created by keqiu on 25-3-23.
//

#include "myQueue.h"

SemaphoreHandle_t xMutex = nullptr;//互斥信号量


BleReceiveQueue receiveData;



void QueueInit()
{
    xMutex = xSemaphoreCreateMutex();

    memset(receiveData.printerBuffer,0,sizeof(receiveData.printerBuffer));
    receiveData.rIndex = 0;
    receiveData.wIndex = 0;
    receiveData.printerLeftLine = 0;
}



uint32_t getLeftLine()
{

    return receiveData.printerLeftLine;
}

/**
 *  写入一行数据到队列缓冲区
 *
 */
void WriteToQueueBuffer(uint8_t* data,uint8_t length)
{
    if (length == 0)
    {
        return;
    }

    if (receiveData.printerLeftLine>MAX_LINE)//剩余行数大于最大行数直接返回
    {
        Serial.printf("剩余行数>最大行数!!");
        return;
    }

    if (length > MAX_ONELINE_BYTE)//如果接收长度超过了一行长度,截断数据为一行
    {
        length = MAX_ONELINE_BYTE;
    }

    memcpy(&receiveData.printerBuffer[receiveData.wIndex],data,length);//将接收的一行数据拷贝到缓冲区中

    receiveData.wIndex = (receiveData.wIndex+1)%MAX_LINE;//写入索引移动1行,并形成环形缓冲区

    receiveData.printerLeftLine++;//打印剩余行数增加1行

    if (receiveData.printerLeftLine >= MAX_LINE)
    {
        receiveData.printerLeftLine = MAX_LINE;//保证打印函数不超过
    }

}

uint8_t* readFromQueueBuffer()
{
     uint32_t rIndex = 0;

    if (receiveData.printerLeftLine>0)
    {
        receiveData.printerLeftLine--;

        rIndex = receiveData.rIndex;//获取当前可读位置的索引

        receiveData.rIndex = (receiveData.rIndex+1)%MAX_LINE;//可读位置索引移动1,形成缓冲区


        return receiveData.printerBuffer[rIndex].lineBuffer;//返回读取位置的一行数据
    }


    return nullptr;
}

void ClearQueueBuffer()
{
    memset(receiveData.printerBuffer,0,sizeof(receiveData.printerBuffer));
    receiveData.rIndex = 0;
    receiveData.wIndex = 0;
    receiveData.printerLeftLine = 0;
}



