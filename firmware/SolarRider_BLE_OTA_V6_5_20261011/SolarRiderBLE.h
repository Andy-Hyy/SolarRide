#pragma once
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <atomic>
#include "SolarRiderOTA.h"
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/task.h>

// A separate task sends telemetry; brake detection never waits for a BLE peer.
static const char* solarService="6E400001-B5A3-F393-E0A9-E50E24DCCA9E";
static const char* solarTX="6E400003-B5A3-F393-E0A9-E50E24DCCA9E";
struct SolarBLELine { uint16_t length; char bytes[256]; };
static QueueHandle_t solarBLEQueue=nullptr;
static BLECharacteristic* solarTXCharacteristic=nullptr;
static std::atomic<bool> solarBLEConnected{false};
static std::atomic<bool> solarBLEAdvertiseAgain{false};

class SolarBLECallbacks:public BLEServerCallbacks {
  void onConnect(BLEServer*) override { solarBLEConnected.store(true); }
  void onDisconnect(BLEServer*) override {
    solarBLEConnected.store(false);solarBLEAdvertiseAgain.store(true);solarOTADisconnected.store(true);
  }
};

static void solarBLETask(void*) {
  SolarBLELine line;
  for (;;) {
    if (solarBLEAdvertiseAgain.exchange(false)) BLEDevice::startAdvertising();
    if (xQueueReceive(solarBLEQueue,&line,pdMS_TO_TICKS(100))!=pdTRUE)continue;
    if(solarOTAMaintenance.load())continue;
    // Conservative 20-byte chunks fit even the minimum ATT MTU (23).
    for (unsigned offset=0;offset<line.length && solarBLEConnected.load();offset+=20) {
      unsigned length=line.length-offset;
      if(length>20)length=20;
      solarTXCharacteristic->setValue((const uint8_t*)line.bytes+offset,length);
      solarTXCharacteristic->notify();
      vTaskDelay(pdMS_TO_TICKS(2));
    }
  }
}

static void solarBLEBegin() {
  solarBLEQueue=xQueueCreate(1,sizeof(SolarBLELine));
  if(!solarBLEQueue){Serial.println("#BLE,ERROR,queue_allocation");return;}
  BLEDevice::init("SolarRider");
  BLEServer* server=BLEDevice::createServer();
  server->setCallbacks(new SolarBLECallbacks());
  BLEService* service=server->createService(solarService);
  solarTXCharacteristic=service->createCharacteristic(solarTX,BLECharacteristic::PROPERTY_NOTIFY);
  solarTXCharacteristic->addDescriptor(new BLE2902());
  service->start();
  solarOTABegin(server);
  BLEAdvertising* advertising=BLEDevice::getAdvertising();
  BLEAdvertisementData advertisingData,scanData;
  advertisingData.setFlags(0x06);advertisingData.setCompleteServices(BLEUUID(solarService));
  advertisingData.setCompleteServices(BLEUUID(uint16_t(0x8018)));
  scanData.setName("SolarRider");
  advertising->setAdvertisementData(advertisingData);
  advertising->setScanResponseData(scanData);
  if(xTaskCreate(solarBLETask,"SolarBLE_TX",4096,nullptr,1,nullptr)!=pdPASS) {
    Serial.println("#BLE,ERROR,task_allocation");return;
  }
  BLEDevice::startAdvertising();
  Serial.println("#BLE,READY,name=SolarRider,transport=BLE,telemetry_hz=10,control=none");
}

static void solarBLEPublish(const char* bytes,int length) {
  static unsigned long lastQueued=0;
  const unsigned long now=millis();
  if(solarOTAMaintenance.load() || !solarBLEConnected.load() || !solarBLEQueue || length<=0 || length>=256 || now-lastQueued<100)return;
  lastQueued=now;
  SolarBLELine line{};line.length=length;memcpy(line.bytes,bytes,length);
  // Keep only the latest row; do not accumulate a queue when the link is slow.
  xQueueOverwrite(solarBLEQueue,&line);
}
