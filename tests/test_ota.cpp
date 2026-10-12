#include "EspBleOTAProtocol.h"
#include <cassert>
#include <vector>
#include <array>
#include <iostream>
#include <fstream>
using namespace SolarOTA;
struct Fake:Driver {
  bool oldBoot=true,begun=false,commitOK=true,writeOK=true,beginOK=true;
  unsigned begins=0,commits=0,aborts=0;std::vector<uint8_t> image;
  std::vector<std::array<uint8_t,20>> replies;
  uint32_t capacity() override{return 0x140000;}
  bool begin(uint32_t)override{++begins;begun=beginOK;image.clear();return beginOK;}
  bool write(uint8_t *p,size_t n)override{if(!writeOK)return false;image.insert(image.end(),p,p+n);return true;}
  bool finish()override{if(!commitOK)return false;++commits;oldBoot=false;return true;}
  void abort()override{++aborts;begun=false;}
  void reply(bool,const uint8_t p[20])override{std::array<uint8_t,20> a;std::copy(p,p+20,a.begin());replies.push_back(a);}
};
std::array<uint8_t,20> cmd(unsigned id,uint32_t size=0){
  std::array<uint8_t,20> p{};put16(p.data(),id);
  for(unsigned i=0;i<4;++i)p[2+i]=size>>(8*i);
  put16(p.data()+18,crc(p.data(),18));return p;
}
std::vector<uint8_t> fixture(unsigned n){std::vector<uint8_t> b(n);for(unsigned i=0;i<n;++i)b[i]=uint8_t(i*13+7);return b;}
void transmit(Receiver &r,const std::vector<uint8_t> &data,unsigned mtu,bool corrupt=false){
  uint32_t t=100;for(unsigned off=0,sector=0;off<data.size();off+=4096,++sector){
    unsigned len=std::min(unsigned(data.size()-off),4096u),step=mtu-5,seq=0;
    for(unsigned b=0;b<len;b+=step){
      unsigned n=std::min(step,len-b);bool tail=b+n==len;
      std::vector<uint8_t> p(3);put16(p.data(),sector);p[2]=tail?0xff:seq++;
      p.insert(p.end(),data.begin()+off+b,data.begin()+off+b+n);
      if(tail){uint16_t c=crc(data.data()+off,len);p.push_back(c);p.push_back(c>>8);if(corrupt)p.back()^=1;}
      r.firmware(p.data(),p.size(),t+=10);if(corrupt&&tail)return;
    }
  }
}
int main(){
  const char *vector="123456789";assert(crc((const uint8_t*)vector,9)==0x31c3);
  auto data=fixture(6200);
  for(unsigned mtu:{22u,182u,244u}){
    Fake f;Receiver r(f);auto a=cmd(1,data.size());r.command(a.data(),20,50,true);
    assert(r.state==Receiver::Receiving&&u16(f.replies.back().data()+4)==0);
    transmit(r,data,mtu);assert(r.received==data.size()&&f.image==data&&f.oldBoot);
    a=cmd(2);r.command(a.data(),20,6000,true);assert(r.state==Receiver::Committed&&f.commits==1&&!f.oldBoot);
    r.disconnect();assert(r.state==Receiver::Committed);
  }
  // Invalid commands cannot start writing, even if they otherwise look valid.
  {Fake f;Receiver r(f);auto a=cmd(1,6200);r.command(a.data(),20,1,false);assert(!f.begins);
   a[18]^=1;r.command(a.data(),20,2,true);assert(!f.begins);
   a=cmd(1,0x140001);r.command(a.data(),20,3,true);assert(!f.begins);
   a=cmd(1,6200);r.command(a.data(),19,4,true);assert(!f.begins);}
  // Incomplete, corrupt, out-of-order and excessive data never select new boot.
  {Fake f;Receiver r(f);auto a=cmd(1,6200);r.command(a.data(),20,1,true);a=cmd(2);r.command(a.data(),20,2,true);
   assert(r.state==Receiver::Failed&&f.oldBoot&&!f.commits);}
  {Fake f;Receiver r(f);auto a=cmd(1,6200);r.command(a.data(),20,1,true);transmit(r,data,182,true);
   assert(r.state==Receiver::Failed&&f.oldBoot&&f.image.empty());}
  {Fake f;Receiver r(f);auto a=cmd(1,6200);r.command(a.data(),20,1,true);uint8_t p[]={1,0,0,9};
   r.firmware(p,4,2);assert(r.state==Receiver::Failed&&f.oldBoot);}
  {Fake f;Receiver r(f);auto a=cmd(1,256);r.command(a.data(),20,1,true);auto b=fixture(257);transmit(r,b,182);
   assert(r.state==Receiver::Failed&&f.oldBoot);}
  {Fake f;Receiver r(f);auto a=cmd(1,6200);r.command(a.data(),20,1,true);f.writeOK=false;transmit(r,data,182);
   assert(r.state==Receiver::Failed&&f.oldBoot);}
  {Fake f;Receiver r(f);auto a=cmd(1,6200);r.command(a.data(),20,1,true);transmit(r,data,182);f.commitOK=false;
   a=cmd(2);r.command(a.data(),20,6000,true);assert(r.state==Receiver::Failed&&f.oldBoot&&!f.commits);}
  // Lost transport and inactivity abort the inactive-slot write.
  for(unsigned cause=0;cause<3;++cause){
   Fake f;Receiver r(f);auto a=cmd(1,6200);r.command(a.data(),20,UINT32_MAX-10000,true);
   if(cause==0)r.disconnect();else if(cause==1)r.transportFailure();else r.tick(25000);
   assert(r.state==Receiver::Failed&&f.oldBoot&&!f.commits);
   r.command(a.data(),20,30000,true);assert(r.state==Receiver::Receiving&&r.received==0);
  }
  std::cout<<"PASS: standard CRC, iPhone-style sectors at ATT 22/182/244, complete commit, parked-only start, oversized/malformed commands, incomplete STOP, CRC/order/length/write/commit failures, disconnect/overflow/rollover timeout, fresh retry\n";
}
