#include "BrakeDetector.h"
#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

const Vec3 base={7.81709629f,-6.35899082f,-2.46035721f};
const Vec3 sensorBias={-0.06863805f,0.00551770f,0.00540442f};
void warmup(BrakeDetector &d,uint32_t &clock,Vec3 resting=base) {
  for(int i=0;i<240;++i) {clock+=10000;d.update(clock,resting,sensorBias);}
  assert(d.ready);
}
Vec3 rotated(Vec3 v,Vec3 axis,float angle) {
  return v*std::cos(angle)+axis.cross(v)*std::sin(angle)
      +axis*(axis.dot(v)*(1-std::cos(angle)));
}
void tests() {
  BrakeDetector d;uint32_t clock=0;warmup(d,clock);
  for(int i=0;i<100;++i){clock+=10000;d.update(clock,base+d.forward*2,sensorBias);assert(!d.braking);}
  // Sustained physical deceleration survives baseline adaptation.
  for(int i=0;i<200;++i){clock+=10000;d.update(clock,base+d.forward*(-1.2f),sensorBias);if(i>40)assert(d.braking);}
  assert(d.events==1);
  for(int i=0;i<100;++i){clock+=10000;d.update(clock,base,sensorBias);}
  assert(!d.braking);
  // A brief impact must not create a brake indication.
  BrakeDetector impact;uint32_t ti=0;warmup(impact,ti);
  for(int i=0;i<2;++i){ti+=10000;impact.update(ti,base+impact.forward*(-3.0f),sensorBias);}
  for(int i=0;i<100;++i){ti+=10000;impact.update(ti,base,sensorBias);}
  assert(impact.events==0);
  // Exact gravity-only pitch rotation should not look like translation.
  BrakeDetector tilt;uint32_t tt=0;warmup(tilt,tt);
  Vec3 axis=base.cross(tilt.forward);axis=axis*(1/axis.norm());
  for(int i=1;i<=200;++i){
    const float angle=0.001f*i;
    Vec3 measured=base*std::cos(angle)+axis.cross(base)*std::sin(angle)
        +axis*(axis.dot(base)*(1-std::cos(angle)));
    tt+=10000;tilt.update(tt,measured,sensorBias+axis*(-0.1f));
    assert(!tilt.braking);assert(std::fabs(tilt.rawForward)<0.03f);
  }
  // Confirm based on elapsed time under irregular sampling and timer rollover.
  BrakeDetector irregular;uint32_t tw=UINT32_MAX-1000000;warmup(irregular,tw);
  float elapsed=0,first=-1;
  for(int i=0;i<90;++i){uint32_t step=i%2?19000:3000;tw+=step;elapsed+=step*1e-6f;
    irregular.update(tw,base+irregular.forward*(-1.2f),sensorBias);
    if(irregular.braking && first<0)first=elapsed;
  }
  assert(first>0.150f && first<0.400f);
  tw+=600000;irregular.update(tw,base,sensorBias);assert(!irregular.ready&&!irregular.braking);
  // A moving startup window is rejected; stable data then calibrates.
  BrakeDetector moving;uint32_t tm=0;
  for(int i=0;i<220;++i){tm+=10000;moving.update(tm,base+moving.forward*(i%2?1.f:-1.f),sensorBias);}
  assert(!moving.ready);warmup(moving,tm);
  // A brief scheduling pause does not destroy an already valid calibration.
  BrakeDetector pause;uint32_t tp=0;warmup(pause,tp);
  tp+=110000;pause.update(tp,base,sensorBias);assert(pause.ready&&!pause.braking);
  // Balanced forward vibration can stitch V4's 700ms holds into endless flash.
  BrakeDetector vibration;uint32_t tv=0;warmup(vibration,tv);
  for(int i=0;i<2000;++i){tv+=10000;
    float disturbance=1.2f*std::sin(2*3.14159265f*2*i*.01f);
    vibration.update(tv,base+vibration.forward*disturbance,sensorBias);
  }
  assert(vibration.events==0);
  // Drift during a vibrating interval, followed by genuine stationary data.
  BrakeDetector drift;uint32_t td=0;warmup(drift,td);
  Vec3 up=base*(1/base.norm());Vec3 pitch=up.cross(drift.forward);pitch=pitch*(1/pitch.norm());
  for(int i=0;i<4500;++i){td+=10000;
    float vertical=.7f*std::sin(2*3.14159265f*5*i*.01f);
    drift.update(td,base+up*vertical,sensorBias+pitch*(-.005f));
  }
  float recovery=-1;
  for(int i=0;i<1500;++i){td+=10000;drift.update(td,base,sensorBias);
    if(!drift.braking&&std::fabs(drift.filteredForward)<.2f&&recovery<0)recovery=(i+1)*.01f;
  }
  assert(recovery>0&&recovery<10);assert(!drift.braking);
  // A genuine 10-second -1.2m/s2 brake is not erased by the recovery branch.
  BrakeDetector sustained;uint32_t tb=0;warmup(sustained,tb);
  for(int i=0;i<1000;++i){tb+=10000;
    sustained.update(tb,base+sustained.forward*(-1.2f),sensorBias);
    if(i>40)assert(sustained.braking);
  }
  // Failed sensor reads cannot be interpreted as deceleration.
  BrakeDetector weak;uint32_t tlight=0;warmup(weak,tlight);
  for(int i=0;i<1000;++i){tlight+=10000;
    weak.update(tlight,base+weak.forward*(-.8f),sensorBias);
    if(i>45)assert(weak.braking);
  }
  assert(weak.events==1);
  sustained.sensorFailure(tb+10000);assert(!sustained.ready&&!sustained.braking);
  assert(sustained.reason==BrakeDetector::Invalid);warmup(sustained,tb);
  std::cout<<"PASS: startup rejection/recovery, 110ms pause, balanced vibration, drift recovery="<<recovery<<"s, 10s genuine brakes at -1.2 and -0.8m/s2, explicit sensor error\n";
  // Starting on uphill/downhill grades must preserve the sign of deceleration.
  for(float grade:{-0.26f,0.26f}) {
    BrakeDetector slope;uint32_t ts=0;
    Vec3 pitchAxis=base.cross(slope.forward);pitchAxis=pitchAxis*(1/pitchAxis.norm());
    Vec3 resting=rotated(base,pitchAxis,grade);warmup(slope,ts,resting);
    for(int i=0;i<100;++i){ts+=10000;slope.update(ts,resting,sensorBias);assert(!slope.braking);}
    for(int i=0;i<200;++i){ts+=10000;slope.update(ts,resting+slope.forward*(-1.2f),sensorBias);if(i>40)assert(slope.braking);}
  }
  // A grade transition without braking, then deceleration during the transition.
  for(float rate:{-0.15f,0.15f}) {
    BrakeDetector hill;uint32_t th=0;warmup(hill,th);
    Vec3 pitchAxis=base.cross(hill.forward);pitchAxis=pitchAxis*(1/pitchAxis.norm());
    for(int i=1;i<=400;++i){
      Vec3 measured=rotated(base,pitchAxis,-rate*0.01f*i);
      if(i>200)measured=measured+hill.forward*(-1.2f);
      th+=10000;hill.update(th,measured,sensorBias+pitchAxis*rate);
      if(i<=200)assert(!hill.braking);
      if(i>240)assert(hill.braking);
    }
  }
  // Bank 30 degrees, turn with 2 m/s2 lateral acceleration, then brake in turn.
  BrakeDetector corner;uint32_t tc=0;warmup(corner,tc);
  Vec3 banked=rotated(base,corner.forward,-0.5235988f);
  for(int i=1;i<=200;++i){tc+=10000;
    corner.update(tc,rotated(base,corner.forward,-0.5235988f*i/200),
                  sensorBias+corner.forward*(0.5235988f/2));assert(!corner.braking);
  }
  Vec3 lateral=banked.cross(corner.forward);lateral=lateral*(1/lateral.norm());
  Vec3 yaw=banked*(0.20f/banked.norm());
  for(int i=0;i<300;++i){tc+=10000;
    Vec3 acceleration=banked+lateral*2.0f;
    if(i>=100)acceleration=acceleration+corner.forward*(-1.2f);
    corner.update(tc,acceleration,sensorBias+yaw);
    if(i<100)assert(!corner.braking);
    if(i>140)assert(corner.braking);
  }
  std::cout<<"PASS: sustained brake, sign, impact, pitch, elapsed time, rollover, gap, stationary calibration, uphill/downhill starts and transitions, banked turning with braking\n";
}

int main(int argc,char **argv) {
  tests();
  if(argc!=3)return 0;
  std::ifstream in(argv[1]);std::ofstream out(argv[2]);std::string line;
  out<<"t_us,ready,forward_raw,forward_filtered,brake,events\n";
  BrakeDetector d;unsigned n=0,brakes=0;
  while(std::getline(in,line)) {
    std::stringstream ss(line);std::string field;std::vector<double> v;
    try {while(std::getline(ss,field,','))v.push_back(std::stod(field));}catch(...){continue;}
    if(v.size()!=15)continue;
    d.update(static_cast<uint32_t>(v[0]),{(float)v[3],(float)v[4],(float)v[5]},
        {(float)v[6],(float)v[7],(float)v[8]});
    out<<(uint32_t)v[0]<<','<<d.ready<<','<<d.rawForward<<','<<d.filteredForward
       <<','<<d.braking<<','<<d.events<<'\n';++n;if(d.braking)++brakes;
  }
  std::cout<<"Replay rows="<<n<<" braking_rows="<<brakes<<" events="<<d.events<<'\n';
}
