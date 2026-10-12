#pragma once
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include <BLESecurity.h>
#include <Update.h>
#include <esp_ota_ops.h>
#include <esp_app_desc.h>
#include <atomic>
#include <freertos/queue.h>
#include "EspBleOTAProtocol.h"
#include "OTAPairing.h"

static constexpr const char *solarOTAVersion="SOLAR_RIDER_BLE_OTA_TRIAL_V6_5";
static constexpr const char *solarOTAMarker="SolarRider_OTA_PROTOCOL_1";
static std::atomic<bool> solarOTAMaintenance{false};
static std::atomic<bool> solarOTADisconnected{false},solarOTAOverflow{false};
static BLECharacteristic *solarOTAFirmware=nullptr,*solarOTACommand=nullptr;
static BLECharacteristic *solarOTAProgress=nullptr,*solarOTAInfo=nullptr;
struct SolarOTAMessage {bool command;uint16_t length;uint8_t bytes[244];};
static QueueHandle_t solarOTAQueue=nullptr;
static bool solarOTAAvailable=false;
static uint32_t solarOTABootGoodSince=0;

class SolarOTAFlash:public SolarOTA::Driver {
  bool first=true,hasMarker=false;size_t matched=0;
 public:
  uint32_t capacity() override {
    const esp_partition_t *p=esp_ota_get_next_update_partition(nullptr);
    return p&&p!=esp_ota_get_running_partition()?p->size:0;
  }
  bool begin(uint32_t bytes) override {
    first=true;hasMarker=false;matched=0;return Update.begin(bytes,U_FLASH);
  }
  bool write(uint8_t *p,size_t n) override {
    if(first){
      first=false;
      // Require an ESP32-C3 application, not a merged/bootloader image.
      if(n<256||p[0]!=0xe9||SolarOTA::u16(p+12)!=5
         ||SolarOTA::u32(p+32)!=ESP_APP_DESC_MAGIC_WORD||p[23]!=1)return false;
    }
    // Future images must retain the OTA receiver; reject a V4/V5 downgrade
    // that would remove wireless update support and require reopening USB.
    for(size_t i=0;i<n;++i){
      if(p[i]==uint8_t(solarOTAMarker[matched]))++matched;
      else matched=p[i]==uint8_t(solarOTAMarker[0])?1:0;
      if(!solarOTAMarker[matched]){hasMarker=true;matched=0;}
    }
    return Update.write(p,n)==n;
  }
  bool finish() override {
    // esp_ota_set_boot_partition inside Update.end verifies the application
    // image, including its embedded SHA256, before changing the boot slot.
    return hasMarker&&Update.end(false);
  }
  void abort() override {Update.abort();}
  void reply(bool command,const uint8_t p[20]) override {
    BLECharacteristic *c=command?solarOTACommand:solarOTAFirmware;
    if(c){c->setValue(p,20);c->notify();}
  }
};
static SolarOTAFlash solarOTAFlash;
static SolarOTA::Receiver solarOTAReceiver(solarOTAFlash);

static bool solarOTAPendingBoot(){
  esp_ota_img_states_t s;
  return esp_ota_get_state_partition(esp_ota_get_running_partition(),&s)==ESP_OK
    &&s==ESP_OTA_IMG_PENDING_VERIFY;
}
static const char *solarOTABootState(){
  esp_ota_img_states_t s;
  if(esp_ota_get_state_partition(esp_ota_get_running_partition(),&s)!=ESP_OK)return "unverified";
  if(s==ESP_OTA_IMG_VALID)return "confirmed";
  if(s==ESP_OTA_IMG_PENDING_VERIFY)return "pending";
  return "unverified";
}
static void solarOTABootFailure(){
  if(solarOTAPendingBoot()){
    Serial.println("#OTA,boot_health_failed,rollback_requested");
    esp_ota_mark_app_invalid_rollback_and_reboot();
  }
}
static void solarOTABootObserve(bool readOK,uint32_t now){
  if(!solarOTAPendingBoot())return;
  if(readOK&&solarOTAAvailable){
    if(!solarOTABootGoodSince)solarOTABootGoodSince=now;
    if(now>5000&&now-solarOTABootGoodSince>=2000){
      if(esp_ota_mark_app_valid_cancel_rollback()==ESP_OK)
        Serial.println("#OTA,boot_health_passed,image_confirmed");
    }
  }else solarOTABootGoodSince=0;
  if(now>30000)solarOTABootFailure();
}
class SolarOTAInfoCallbacks:public BLECharacteristicCallbacks {
  void onRead(BLECharacteristic *c) override {
    char elf[65]={};esp_app_get_elf_sha256(elf,sizeof(elf));char line[220];
    const int n=snprintf(line,sizeof(line),
      "{\"version\":\"%s\",\"protocol\":\"%s\",\"elf_sha256\":\"%s\",\"boot\":\"%s\"}",
      solarOTAVersion,solarOTAMarker,elf,solarOTABootState());
    c->setValue((const uint8_t*)line,n);
  }
};
class SolarOTAWriteCallbacks:public BLECharacteristicCallbacks {
  const bool isCommand;
 public:
  explicit SolarOTAWriteCallbacks(bool command):isCommand(command){}
  void onWrite(BLECharacteristic *c) override {
    const String value=c->getValue();
    if(!solarOTAQueue||value.length()>244||value.length()==0){solarOTAOverflow=true;return;}
    SolarOTAMessage p{};p.command=isCommand;p.length=value.length();
    memcpy(p.bytes,value.c_str(),p.length);
    // Flash operations run in loop(), never in the Bluetooth callback task.
    if(xQueueSend(solarOTAQueue,&p,0)!=pdTRUE)solarOTAOverflow=true;
  }
};
static void solarOTABegin(BLEServer *server){
  solarOTAQueue=xQueueCreate(32,sizeof(SolarOTAMessage));
  if(!solarOTAQueue){Serial.println("#OTA,ERROR,queue_allocation");return;}
  BLEDevice::setMTU(247);
  BLESecurity::setAuthenticationMode(true,true,true);
  BLESecurity::setCapability(ESP_IO_CAP_OUT);
  BLESecurity::setPassKey(true,solarOTAPairingPIN);
  BLEService *service=server->createService(BLEUUID(uint16_t(0x8018)));
  const uint32_t writeProps=BLECharacteristic::PROPERTY_WRITE
    |BLECharacteristic::PROPERTY_NOTIFY|BLECharacteristic::PROPERTY_WRITE_AUTHEN;
  solarOTAFirmware=service->createCharacteristic(BLEUUID(uint16_t(0x8020)),writeProps);
  solarOTACommand=service->createCharacteristic(BLEUUID(uint16_t(0x8022)),writeProps);
#if defined(CONFIG_BLUEDROID_ENABLED)
  solarOTAFirmware->setAccessPermissions(ESP_GATT_PERM_WRITE_ENC_MITM);
  solarOTACommand->setAccessPermissions(ESP_GATT_PERM_WRITE_ENC_MITM);
#endif
  solarOTAFirmware->addDescriptor(new BLE2902());
  solarOTACommand->addDescriptor(new BLE2902());
  solarOTAFirmware->setCallbacks(new SolarOTAWriteCallbacks(false));
  solarOTACommand->setCallbacks(new SolarOTAWriteCallbacks(true));
  solarOTAProgress=service->createCharacteristic(BLEUUID(uint16_t(0x8021)),
    BLECharacteristic::PROPERTY_READ|BLECharacteristic::PROPERTY_NOTIFY);
  solarOTAProgress->addDescriptor(new BLE2902());
  const uint8_t zero[2]={};solarOTAProgress->setValue(zero,2);
  solarOTAInfo=service->createCharacteristic(BLEUUID(uint16_t(0x8023)),BLECharacteristic::PROPERTY_READ);
  solarOTAInfo->setCallbacks(new SolarOTAInfoCallbacks());
  service->start();solarOTAAvailable=true;
  Serial.println("#OTA,READY,profile=Espressif_8018,pairing_required=1");
}
static void solarOTAPoll(bool allowStart,uint32_t now){
  if(!solarOTAAvailable)return;
  if(solarOTADisconnected.exchange(false)){
    solarOTAReceiver.disconnect();xQueueReset(solarOTAQueue);
  }
  if(solarOTAOverflow.exchange(false)){
    solarOTAReceiver.transportFailure();xQueueReset(solarOTAQueue);
  }
  SolarOTAMessage p;
  for(unsigned i=0;i<8&&xQueueReceive(solarOTAQueue,&p,0)==pdTRUE;++i){
    if(p.command)solarOTAReceiver.command(p.bytes,p.length,now,allowStart&&!solarOTAPendingBoot());
    else solarOTAReceiver.firmware(p.bytes,p.length,now);
  }
  solarOTAReceiver.tick(now);
  solarOTAMaintenance.store(solarOTAReceiver.maintenance());
  static uint32_t previousProgress=UINT32_MAX;
  const uint32_t pct=solarOTAReceiver.total
    ?uint32_t(uint64_t(solarOTAReceiver.received)*10000/solarOTAReceiver.total):0;
  if(pct!=previousProgress){
    previousProgress=pct;const uint8_t bytes[2]={uint8_t(pct/100),uint8_t(pct%100)};
    solarOTAProgress->setValue(bytes,2);solarOTAProgress->notify();
  }
  if(solarOTAReceiver.state==SolarOTA::Receiver::Committed
     &&uint32_t(now-solarOTAReceiver.lastActivity)>=1500)ESP.restart();
}
