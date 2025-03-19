//
// Created by keqiu on 25-3-16.
//
#include "BLE.h"
#include <Arduino.h>

#include <BLEDevice.h>
#include <BLEServer.h>
#include <device.h>
#include "led.h"

#define DEVICE_NAME "Mini-Printer"
#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b" //自定义服务UUID
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8" //自定义特征UUID


bool BLEConnected = false;

BLECharacteristic *pCharacteristic;

class MyServerCallbacks : public BLEServerCallbacks
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

class MyCharacteristicCallbacks : public BLECharacteristicCallbacks
{

    void onRead(BLECharacteristic *pCharacteristic) override
    {
        Serial.println("触发读取事件");
    }

    void onWrite(BLECharacteristic *pCharacteristic) override
    {
        Serial.println("触发写入事件");
    }
};


void BLEInit()
{
    //1.初始化BLE设备
    BLEDevice::init(DEVICE_NAME);

    //2.创建BLE服务器
    BLEServer *pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    //3.创建BLE服务Service
    BLEService *pService = pServer->createService(SERVICE_UUID);

    //4.创建特征，权限可读可写可通知
    pCharacteristic = pService->createCharacteristic(
                                CHARACTERISTIC_UUID,
                    BLECharacteristic::PROPERTY_READ |
                             BLECharacteristic::PROPERTY_WRITE|
                             BLECharacteristic::PROPERTY_NOTIFY
    );

   pCharacteristic->setValue("test data");//设置特征值
   pCharacteristic->setCallbacks(new MyCharacteristicCallbacks());//设置特征回调

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

        status[0] = pDevice->battery;
        status[1] = pDevice->temperature;
        status[2] = pDevice->paperStatus;
        status[3] = pDevice->printerStatus;

        pCharacteristic->setValue(status,  sizeof(status));
        pCharacteristic->notify();//通知给连接设备,连接订阅即可收到消息

        Serial.println("状态上报成功");
    }
}