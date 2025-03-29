//
// Created by keqiu on 25-3-23.
//

#ifndef MYQUEUE_H
#define MYQUEUE_H

#include <Arduino.h>

#define MAX_ONELINE_BYTE 48 //打印时一行最大字节数

#define MAX_LINE 1000//最多存1000行数据

typedef struct
{
    uint8_t lineBuffer[MAX_ONELINE_BYTE];
}OneLineBuffer;

typedef struct
{
    OneLineBuffer printerBuffer[MAX_LINE];
    uint32_t printerLeftLine;//剩余打印行数
    uint32_t rIndex;
    uint32_t wIndex;
}BleReceiveQueue;

void QueueInit();
void ClearQueueBuffer();

uint32_t getLeftLine();

void WriteToQueueBuffer(uint8_t* data,uint8_t length);
uint8_t* readFromQueueBuffer();





#endif //MYQUEUE_H
