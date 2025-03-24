//
// Created by keqiu on 25-3-23.
//

#ifndef MYBUFFER_H
#define MYBUFFER_H

#include <Arduino.h>

#define MAX_BUFFER_SIZE 48*100 //一行48个Byte,最大缓冲100行

typedef struct
{
    uint8_t arrayBuffer[MAX_BUFFER_SIZE];
    size_t bufferIndex;
}ArrayBuffer;


void WriteToArrayBuffer(uint8_t* data,uint8_t length);

ArrayBuffer* getPrinterArrayBuffer();

void ClearPrinterBuffer();
#endif //MYBUFFER_H
