#pragma once
#include <cstdint>
#include <cstddef>
#include <cstring>
#include <algorithm>

// Espressif public BLE OTA wire format (service 0x8018). This parser owns
// ordering, CRC, size and commit rules independently of the BLE/flash driver.
namespace SolarOTA {
inline uint16_t u16(const uint8_t *p){return uint16_t(p[0])|(uint16_t(p[1])<<8);}
inline uint32_t u32(const uint8_t *p){return u16(p)|(uint32_t(u16(p+2))<<16);}
inline void put16(uint8_t *p,uint16_t n){p[0]=n;p[1]=n>>8;}
inline uint16_t crc(const uint8_t *p,size_t n){
  uint16_t c=0;while(n--){c^=uint16_t(*p++)<<8;
    for(unsigned i=0;i<8;++i)c=c&0x8000?uint16_t((c<<1)^0x1021):uint16_t(c<<1);
  }return c;
}
struct Driver {
  virtual uint32_t capacity()=0;
  virtual bool begin(uint32_t bytes)=0;
  virtual bool write(uint8_t *data,size_t bytes)=0;
  virtual bool finish()=0;
  virtual void abort()=0;
  virtual void reply(bool command,const uint8_t packet[20])=0;
  virtual ~Driver()=default;
};
class Receiver {
 public:
  enum State:uint8_t {Idle,Receiving,Committed,Failed};
  explicit Receiver(Driver &d):driver(d){}
  State state=Idle;
  uint32_t total=0,received=0,lastActivity=0;
  uint16_t sector=0;
  uint8_t error=0;
  bool maintenance()const{return state==Receiving||state==Committed;}
  void command(const uint8_t *p,size_t n,uint32_t now,bool allowStart){
    if(n!=20)return;
    const uint16_t id=u16(p);
    if(crc(p,18)!=u16(p+18)){cmdAck(id,1);return;}
    if(id==1){
      const uint32_t size=u32(p+2);
      if(maintenance()||!allowStart||size<256||size>driver.capacity()){
        cmdAck(1,1);return;
      }
      total=size;received=0;sector=0;offset=0;sequence=0;error=0;
      if(!driver.begin(size)){fail(5);cmdAck(1,1);return;}
      state=Receiving;lastActivity=now;cmdAck(1,0);
    }else if(id==2){
      // STOP means finalize in Espressif's app; incomplete STOP is rejected.
      if(state!=Receiving||received!=total||offset!=0){
        if(state==Receiving)fail(6);cmdAck(2,1);return;
      }
      if(!driver.finish()){fail(7);cmdAck(2,1);return;}
      state=Committed;lastActivity=now;cmdAck(2,0);
    }else if(id==4){ // Optional Mac cancellation, outside the official START/STOP pair.
      if(state==Receiving)fail(8);cmdAck(4,0);
    }else cmdAck(id,1);
  }
  void firmware(const uint8_t *p,size_t n,uint32_t now){
    if(state!=Receiving)return;
    if(received>=total){fail(3);fwAck(sector,3);return;}
    if(n<4||n>244){fail(3);fwAck(sector,3);return;}
    const uint16_t index=u16(p);const uint8_t seq=p[2];
    if(index!=sector){fail(2);fwAck(index,2);return;}
    bool tail=seq==0xff;
    if(!tail&&seq!=sequence){fail(3);fwAck(index,3);return;}
    if(tail&&n<5){fail(3);fwAck(index,3);return;}
    const size_t length=n-3-(tail?2:0);
    const uint32_t expected=std::min(uint32_t(4096),total-received);
    if(offset+length>expected){fail(3);fwAck(index,3);return;}
    std::memcpy(buffer+offset,p+3,length);offset+=length;lastActivity=now;
    if(!tail){
      // 0xff is reserved for a sector tail. A minimum 22-byte ATT payload
      // is needed for a full 4096-byte sector without exhausting sequence IDs.
      if(sequence>=254){fail(3);fwAck(index,3);return;}
      ++sequence;return;
    }
    if(offset!=expected){fail(3);fwAck(index,3);return;}
    if(crc(buffer,offset)!=u16(p+n-2)){fail(1);fwAck(index,1);return;}
    if(!driver.write(buffer,offset)){fail(5);fwAck(index,3);return;}
    received+=offset;++sector;offset=0;sequence=0;fwAck(index,0);
  }
  void tick(uint32_t now){if(state==Receiving&&uint32_t(now-lastActivity)>30000)fail(9);}
  void disconnect(){if(state==Receiving)fail(10);}
  void transportFailure(){if(state==Receiving)fail(11);}
 private:
  Driver &driver;
  uint8_t buffer[4096]={};uint16_t offset=0;uint8_t sequence=0;
  void fail(uint8_t why){driver.abort();state=Failed;error=why;offset=0;sequence=0;}
  void cmdAck(uint16_t id,uint16_t result){
    uint8_t p[20]={};put16(p,3);put16(p+2,id);put16(p+4,result);
    if(id==1&&result==0)p[6]=1; // Send one sector, then wait for its flash ACK.
    put16(p+18,crc(p,18));driver.reply(true,p);
  }
  void fwAck(uint16_t index,uint16_t result){
    uint8_t p[20]={};put16(p,index);put16(p+2,result);put16(p+4,sector);
    put16(p+18,crc(p,18));driver.reply(false,p);
  }
};
}
