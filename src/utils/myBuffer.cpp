//
// Created by keqiu on 25-3-23.
//

#include "myBuffer.h"


ArrayBuffer printerBuffer;

void WriteToArrayBuffer(uint8_t* data,uint8_t length)
{
        size_t remainSize = MAX_BUFFER_SIZE-printerBuffer.bufferIndex;
        if (length>remainSize)
        {
                Serial.printf("缓冲区溢出,自动截断数据\r\n");
                length = remainSize;
        }

        if (length>0)
        {
                memcpy(printerBuffer.arrayBuffer,data,length);
                printerBuffer.bufferIndex += length;

                Serial.printf("写入%d个字节,当前写入位置为：%d",length,printerBuffer.bufferIndex);
        }

}

ArrayBuffer* getPrinterArrayBuffer()
{
      return &printerBuffer;
}

void ClearPrinterBuffer()
{
        memset(printerBuffer.arrayBuffer,0,sizeof(printerBuffer.arrayBuffer));
        printerBuffer.bufferIndex = 0;
}