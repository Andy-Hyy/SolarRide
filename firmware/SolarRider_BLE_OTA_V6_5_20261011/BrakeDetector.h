#pragma once
#include <cmath>
#include <cstdint>

struct Vec3 {
  float x, y, z;
  Vec3 operator+(Vec3 b) const { return {x+b.x,y+b.y,z+b.z}; }
  Vec3 operator-(Vec3 b) const { return {x-b.x,y-b.y,z-b.z}; }
  Vec3 operator*(float k) const { return {x*k,y*k,z*k}; }
  float dot(Vec3 b) const { return x*b.x+y*b.y+z*b.z; }
  float norm() const { return std::sqrt(dot(*this)); }
  Vec3 cross(Vec3 b) const { return {y*b.z-z*b.y,z*b.x-x*b.z,x*b.y-y*b.x}; }
};

class BrakeDetector {
 public:
  // Measured from the current upright front-wheel lift; tolerate small placement changes.
  const Vec3 forward={-0.5883380f,-0.7899954f,0.1725274f};
  static constexpr float threshold=0.70f;
  static constexpr float confirmationSeconds=0.150f;
  static constexpr float holdSeconds=0.700f;
  // Codes in CSV/metadata: 0 ready, 1 gathering, 2 moving accel,
  // 3 moving gyro, 4 invalid/read failure, 5 long gap, 6 bad magnitude.
  enum Reason : uint8_t { OK, Gathering, MovingAccel, MovingGyro, Invalid, Gap, Magnitude };
  bool ready=false, braking=false;
  Vec3 gravity={},bias={},linear={};
  float rawForward=0,filteredForward=0,confirmSeconds=0;
  float accelStdMax=0,gyroStdMax=0,gravityError=0;
  uint32_t events=0,sensorFailures=0,gaps=0;
  Reason reason=Gathering;
  // After a maintenance session, acquire a fresh stationary reference.
  void restartCalibration(){invalidate(Gathering);}

  void sensorFailure(uint32_t now) {
    ++sensorFailures;invalidate(Invalid);previousUs=now;haveTime=true;
  }
  void update(uint32_t now,Vec3 a,Vec3 gyro) {
    if(!finite(a)||!finite(gyro)||a.norm()<1||a.norm()>80||gyro.norm()>20) {
      sensorFailure(now);return;
    }
    if(!haveTime){previousUs=now;haveTime=true;addSample(now,a,gyro);return;}
    const uint32_t elapsed=now-previousUs;previousUs=now;
    if(elapsed>500000){++gaps;invalidate(Gap);addSample(now,a,gyro);return;}
    if(elapsed==0)return;
    const float dt=elapsed*1e-6f;
    const bool interrupted=elapsed>50000;
    if(interrupted){++gaps;confirmSeconds=0;clearWindow();}
    addSample(now,a,gyro);
    if(!ready){calibrate();return;}
    const Vec3 omega=gyro-bias;
    const float speed=omega.norm();
    if(speed>1e-7f){
      const Vec3 axis=omega*(-1/speed);const float angle=speed*dt;
      gravity=gravity*std::cos(angle)+axis.cross(gravity)*std::sin(angle)
        +axis*(axis.dot(gravity)*(1-std::cos(angle)));
    }
    normalizeGravity();

    // Correction is never frozen by the brake flag. Weak continuous fusion
    // bounds drift; a stable, magnitude-consistent raw window allows recovery
    // even when the old linear estimate wrongly reports a persistent brake.
    bool quiet=windowValid && accelStdMax<0.06f && gyroStdMax<0.008f
      && windowChange<0.035f && (meanG-bias).norm()<0.025f
      && std::fabs(meanA.norm()-gravityMagnitude)<0.020f;
    if(quiet){
      const Vec3 target=meanA*(gravityMagnitude/meanA.norm());
      gravity=gravity+(target-gravity)*(1-std::exp(-dt/1.5f));
      bias=bias+(meanG-bias)*(1-std::exp(-dt/10.0f));
    }else if(windowValid && std::fabs(meanA.norm()-gravityMagnitude)<0.020f
             && std::fabs(a.norm()-gravityMagnitude)<0.75f){
      // A stable sustained deceleration changes the mean magnitude. Do not
      // absorb that signal into gravity just because its per-axis noise is low.
      const Vec3 target=a*(gravityMagnitude/a.norm());
      gravity=gravity+(target-gravity)*(1-std::exp(-dt/20.0f));
    }
    normalizeGravity();
    linear=a-gravity;gravityError=linear.norm();rawForward=linear.dot(forward);
    filteredForward+=(rawForward-filteredForward)*(1-std::exp(-dt/0.035f));
    // Do not count an unobserved interval as continuous braking evidence.
    if(!interrupted && filteredForward<-threshold)confirmSeconds+=dt;
    else confirmSeconds=0;
    holdRemaining=std::fmax(0.0f,holdRemaining-dt);
    if(confirmSeconds>=confirmationSeconds)holdRemaining=holdSeconds;
    const bool next=holdRemaining>0;
    if(next&&!braking)++events;
    braking=next;reason=OK;
  }

 private:
  static constexpr unsigned N=128;
  Vec3 samplesA[N]={},samplesG[N]={};
  uint32_t times[N]={},previousUs=0,lastWindowUs=0;
  unsigned count=0,head=0;
  bool haveTime=false,windowValid=false;
  Vec3 meanA={},meanG={};
  float gravityMagnitude=0,holdRemaining=0,windowChange=0;
  static bool finite(Vec3 v){return std::isfinite(v.x)&&std::isfinite(v.y)&&std::isfinite(v.z);}
  void clearWindow(){count=head=0;windowValid=false;accelStdMax=gyroStdMax=0;}
  void invalidate(Reason cause){
    ready=braking=false;rawForward=filteredForward=confirmSeconds=holdRemaining=gravityError=0;
    clearWindow();reason=cause;
  }
  void normalizeGravity(){const float n=gravity.norm();if(n>1)gravity=gravity*(gravityMagnitude/n);}
  void addSample(uint32_t now,Vec3 a,Vec3 g){
    if(count && now-lastWindowUs<10000)return;
    lastWindowUs=now;samplesA[head]=a;samplesG[head]=g;times[head]=now;
    head=(head+1)%N;if(count<N)++count;
    if(count<N)return;
    if(now-times[head]<1200000)return;
    meanA={};meanG={};Vec3 first={},last={};
    for(unsigned i=0;i<N;++i){unsigned j=(head+i)%N;
      meanA=meanA+samplesA[j]*(1.0f/N);meanG=meanG+samplesG[j]*(1.0f/N);
      if(i<N/2)first=first+samplesA[j]*(2.0f/N);
      else last=last+samplesA[j]*(2.0f/N);
    }
    Vec3 va={},vg={};
    for(unsigned i=0;i<N;++i){Vec3 da=samplesA[i]-meanA,dg=samplesG[i]-meanG;
      va=va+Vec3{da.x*da.x,da.y*da.y,da.z*da.z};
      vg=vg+Vec3{dg.x*dg.x,dg.y*dg.y,dg.z*dg.z};
    }
    accelStdMax=std::sqrt(std::fmax(va.x,std::fmax(va.y,va.z))/(N-1));
    gyroStdMax=std::sqrt(std::fmax(vg.x,std::fmax(vg.y,vg.z))/(N-1));
    windowChange=(last-first).norm();windowValid=true;
  }
  void calibrate(){
    if(!windowValid){if(reason!=Invalid&&reason!=Gap)reason=Gathering;return;}
    if(meanA.norm()<8||meanA.norm()>12){reason=Magnitude;return;}
    if(accelStdMax>=0.06f||windowChange>=0.035f){reason=MovingAccel;return;}
    if(gyroStdMax>=0.008f||meanG.norm()>0.25f){reason=MovingGyro;return;}
    gravity=meanA;gravityMagnitude=meanA.norm();bias=meanG;ready=true;reason=OK;
  }
};
