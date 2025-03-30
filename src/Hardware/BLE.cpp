//
// Created by keqiu on 25-3-16.
//
#include "BLE.h"
#include <Arduino.h>

#include "utils/myQueue.h"
#include "utils/myBuffer.h"
#include <BLEDevice.h>
#include <BLEServer.h>
#include "device.h"
#include "led.h"

#define DEVICE_NAME "Mini-Printer"
#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b" //自定义服务的UUID
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8" //自定义特征的UUID


bool BLEConnected = false;

BLECharacteristic *pCharacteristic;


class IsConnectServerCallbacks : public BLEServerCallbacks
{
    void onConnect(BLEServer *pServer) override
    {
        BLEConnected = true;
        LedControl(LED_ON);
        Serial.println("现在有设备接入~");
    }

    void onDisconnect(BLEServer *pServer) override
    {
        BLEConnected = false;
        LedControl(LED_OFF);

        Serial.println("设备断开连接~");
        pServer->startAdvertising();//由于BLE特性在设备连接后会关闭广播，所以这里需要重新开启，否则设备无法搜索到

    }
};

class TriggerCharacteristicCallbacks : public BLECharacteristicCallbacks
{

    void onRead(BLECharacteristic *pCharacteristic) override
    {
        Serial.println("触发读取事件");
    }

    void onWrite(BLECharacteristic *pCharacteristic) override
    {
          uint8_t* pData = pCharacteristic->getData();//pData指向特征值缓冲区的首地址
          size_t length = pCharacteristic->getLength();
          if (length == 5)
          {
              if (pData[0]==0xA5 && pData[1]==0xA5 && pData[2]==0xA5 && pData[3]==0xA5)
              {
                  //设置打印深度
                  switch (pData[4])
                  {
                      case 30:
                          break;
                      case 60:
                          break;
                      case 100:
                          break;
                      default:
                          break;
                  }
                  return ; //设置深度后立马返回不处理其他
              }
              else if (pData[0]==0xA6&&pData[1]==0xA6&&pData[2]==0xA6&&pData[3]==0xA6 && pData[4]==0x01)
              {
                    setBleReadFlag(true);//接收到该信息代表数据接收完成了,触发打印
                    Serial.printf("接收数据完成,总行数:\r\n");
              }
          }


        /**
         * 写入数据到缓冲区
         * 此处与安卓客户端协商,每次安卓发送的数据包只包含一行数据48Byte
         */
        WriteToQueueBuffer(pData,length);
        // WriteToArrayBuffer(pData,length);

        Serial.println("触发写入事件");
    }
};


void BLEInit()
{
    //1.初始化BLE设备
    BLEDevice::init(DEVICE_NAME);

    //2.创建BLE服务器
    BLEServer *pServer = BLEDevice::createServer();
    pServer->setCallbacks(new IsConnectServerCallbacks());//绑定回调

    //3.创建BLE服务Service
    BLEService *pService = pServer->createService(SERVICE_UUID);

    //4.创建特征，权限可读可写可通知
    pCharacteristic = pService->createCharacteristic(
                                CHARACTERISTIC_UUID,
                    BLECharacteristic::PROPERTY_READ |
                             BLECharacteristic::PROPERTY_WRITE|
                             BLECharacteristic::PROPERTY_NOTIFY
    );

   pCharacteristic->setValue("PRINTER");//设置特征值,客户端连接上后没有任何写入读取到的默认值
   pCharacteristic->setCallbacks(new TriggerCharacteristicCallbacks());//设置特征回调

    //5.启动服务
    pService->start();

    //6.启动广播
    pServer->startAdvertising();

}

void BLEReport()
{
    if (BLEConnected)
    {
        DeviceStatus* pDevice = getDeviceStatus();

        uint8_t status[4] = {0};

        //简易数据包,不考虑安全
        //包头 长度 指令类型 和校验码都没有,实际使用应该要加上

        status[0] = pDevice->battery;
        status[1] = pDevice->temperature;
        status[2] = pDevice->paperStatus;
        status[3] = pDevice->printerStatus;

        pCharacteristic->setValue(status,  sizeof(status));
        pCharacteristic->notify();//通知给连接设备,连接订阅即可收到消息

        // Serial.println("状态上报成功");
    }
}
