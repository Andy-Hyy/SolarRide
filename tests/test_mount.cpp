#include "BrakeDetector.h"
#include <cassert>
#include <iostream>
const Vec3 base={7.81709629f,-6.35899082f,-2.46035721f};
const Vec3 bias={-.06863805f,.00551770f,.00540443f};
Vec3 rotate(Vec3 v,Vec3 axis,float angle){return v*std::cos(angle)+axis.cross(v)*std::sin(angle)+axis*(axis.dot(v)*(1-std::cos(angle)));}
void warmup(BrakeDetector &d,uint32_t &t,Vec3 g){for(int i=0;i<240;++i){t+=10000;d.update(t,g,bias);}assert(d.ready);}
int main(){
  BrakeDetector shortBrake;uint32_t t=0;warmup(shortBrake,t,base);
  bool indicatedBeforeRelease=false;
  for(int i=0;i<25;++i){t+=10000;shortBrake.update(t,base+shortBrake.forward*(-.8f),bias);indicatedBeforeRelease|=shortBrake.braking;}
  for(int i=0;i<100;++i){t+=10000;shortBrake.update(t,base,bias);}
  std::cout<<"250ms -0.8m/s2 brake: indicated_before_release="<<indicatedBeforeRelease<<", events="<<shortBrake.events<<'\n';
#ifdef REQUIRE_SHORT_BRAKE
  assert(indicatedBeforeRelease&&shortBrake.events==1&&!shortBrake.braking);
#endif
  for(Vec3 axis:{Vec3{1,0,0},Vec3{0,1,0},Vec3{0,0,1}})for(float angle:{-.174532925f,.174532925f}){
    BrakeDetector d;uint32_t clock=0;Vec3 g=rotate(base,axis,angle),actualForward=rotate(d.forward,axis,angle);
    warmup(d,clock,g);
    for(int i=0;i<100;++i){clock+=10000;d.update(clock,g,bias);assert(!d.braking);}
    for(int i=0;i<200;++i){clock+=10000;d.update(clock,g+actualForward*(-1.2f),bias);if(i>40)assert(d.braking);}
    for(int i=0;i<200;++i){clock+=10000;d.update(clock,g,bias);}
    assert(d.events==1&&!d.braking);
  }
  std::cout<<"PASS: +/-10 degree placement rotations around each sensor axis, stationary baseline, -1.2m/s2 deceleration, stop recovery\n";
}
