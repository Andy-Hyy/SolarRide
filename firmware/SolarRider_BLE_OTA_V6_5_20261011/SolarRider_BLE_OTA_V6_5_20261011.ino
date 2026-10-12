#include <Adafruit_NeoPixel.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include "BrakeDetector.h"
#include "SolarRiderBLE.h"
#include "CheckedIMU.h"

BrakeDetector brakeDetector;

// Arduino 3.3.10 otherwise confirms an OTA image before setup().
extern "C" bool verifyRollbackLater(void){return true;}

// ==========================================
// 🛠️ 核心引脚配置区 (已根据原理图与实物图严格对应)
// ==========================================
#define MAIN_LED_PIN    8   // 主大灯带数据引脚 (对应 LED_Strip_DATA_RAW, GPIO08)
#define STATUS_LED_PIN  2   // 状态指示灯数据引脚 (对应 STATUS_LED_RAW, GPIO02)
#define PIN_LEFT        3   // 遥控器换向后：GPIO03 对应左转操作
#define PIN_RIGHT       6   // 遥控器换向后：GPIO06 对应右转操作
#define PIN_STOP        10  // 停止(重置)遥控引脚 (对应 PIN_STOP, GPIO10)
#define I2C_SDA         4   // 陀螺仪 SDA 引脚 (对应 SDA, GPIO04)
#define I2C_SCL         5   // 陀螺仪 SCL 引脚 (对应 SCL, GPIO05)
// ==========================================

#define NUM_PIXELS    10

// 实例化主灯带与状态指示灯
Adafruit_NeoPixel strip(NUM_PIXELS, MAIN_LED_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel statusLed(1, STATUS_LED_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_MPU6050 mpu;

// 状态机变量：0常规骑行，1左转流水，2右转流水
int currentMode = 0; 

// 异步时间控制变量
unsigned long lastUpdateTime = 0;
int animationStep = 0;
const int frameDelay = 60;   
const int pauseDelay = 300;  
bool isPausing = false;

// 模式0常规时间
unsigned long lastMode0Time = 0;

// 刹车控制变量
bool flashState = false;
unsigned long lastFlashTime = 0;

// 核心自适应滤波变量
float baselineY = 0.0;
const float brakeThreshold = 4.0;    // 空间阈值
const int requiredHits = 5;          // 时间阈值
int hitCounter = 0;                  // 连续命中计数器

// 状态灯呼吸控制变量
unsigned long lastStatusTime = 0;
bool statusBlink = false;

// Nonblocking comparison telemetry: original Y detector remains visible in logs.
// USB monitor: up to 50 data rows per second; calibration summary every second.
// Sensor sampling, brake decisions, and BLE telemetry retain their own timing.
const unsigned long serialOutputIntervalMs = 20;
const unsigned long calibrationSummaryIntervalMs = 1000;
unsigned long debugLastSerialMs=0;
unsigned long debugLastRowMs=0, debugLastBaseMs=0, debugLastSampleUs=0;
unsigned long debugSequence=0, debugDropped=0;

void debugTryWrite(const char *line, int length) {
  if (length>0 && length<256 && Serial.availableForWrite()>=length)
    Serial.write((const uint8_t*)line,(size_t)length);
  else debugDropped++;
}

void debugTelemetry(const sensors_event_t &a, const sensors_event_t &g,
                    float diffY, bool oldBraking,
                    unsigned long sampleUs, unsigned long loopDtUs) {
  const unsigned long now=millis();
  if (now-debugLastRowMs<20) return;
  debugLastRowMs=now;
  char line[256];
  if (now-debugLastBaseMs>=calibrationSummaryIntervalMs) {
    debugLastBaseMs=now;
    int n=snprintf(line,sizeof(line),
      "#CAL,ready=%d,reason=%u,acc_sd=%.4f,gyro_sd=%.5f,read_errors=%lu,gaps=%lu,bias_x=%.5f,bias_y=%.5f,bias_z=%.5f,threshold=%.2f,confirm_s=%.3f,hold_s=%.3f,road_validated=0\n",
      brakeDetector.ready?1:0,(unsigned)brakeDetector.reason,
      brakeDetector.accelStdMax,brakeDetector.gyroStdMax,
      (unsigned long)brakeDetector.sensorFailures,(unsigned long)brakeDetector.gaps,
      brakeDetector.bias.x,brakeDetector.bias.y,brakeDetector.bias.z,
      BrakeDetector::threshold,BrakeDetector::confirmationSeconds,
      BrakeDetector::holdSeconds);
    debugTryWrite(line,n);
  }
  int n=snprintf(line,sizeof(line),
    "%lu,%lu,%lu,%.3f,%.3f,%.3f,%.4f,%.4f,%.4f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%d,%d,%d,%d,%lu,%d,%lu,%u,%.4f,%.5f,%.3f\n",
    sampleUs,debugSequence,loopDtUs,a.acceleration.x,a.acceleration.y,
    a.acceleration.z,g.gyro.x,g.gyro.y,g.gyro.z,
    brakeDetector.gravity.x,brakeDetector.gravity.y,brakeDetector.gravity.z,
    brakeDetector.rawForward,brakeDetector.filteredForward,diffY,hitCounter,
    oldBraking?1:0,brakeDetector.ready?1:0,brakeDetector.braking?1:0,
    (unsigned long)brakeDetector.events,currentMode,debugDropped,
    (unsigned)brakeDetector.reason,brakeDetector.accelStdMax,
    brakeDetector.gyroStdMax,brakeDetector.gravityError);
  if (now-debugLastSerialMs>=serialOutputIntervalMs) {
    debugLastSerialMs=now;
    debugTryWrite(line,n);
  }
  solarBLEPublish(line,n);
}

void setup() {
  // Diagnostic UART buffer accommodates the added gyro columns without waiting.
  Serial.setTxBufferSize(512);
  Serial.begin(115200);
  Serial.println("#TELEMETRY,SOLAR_RIDER_BLE_OTA_TRIAL_V6_5,serial_hz=50,ble_hz=10,accel=m_s2,gyro=rad_s,startup=keep_stationary_2s,road_validated=0");
  Serial.println("t_us,seq,loop_dt_us,ax,ay,az,gx,gy,gz,gravity_x,gravity_y,gravity_z,forward_raw,forward_filtered,old_diff_y,old_hits,old_brake,ready,brake,events,mode,dropped,cal_reason,acc_std_max,gyro_std_max,gravity_error");
  Serial.printf("#MOUNT,fx=%.7f,fy=%.7f,fz=%.7f,calibration=front_wheel_lift_20261011\n",
    brakeDetector.forward.x,brakeDetector.forward.y,brakeDetector.forward.z);
  
  // 1. 初始化主灯带和状态灯
  strip.begin();
  strip.show();
  statusLed.begin();
  statusLed.setPixelColor(0, statusLed.Color(50, 50, 0)); // 初始化时亮微弱黄光
  statusLed.show();

  // 2. 初始化输入引脚
  pinMode(PIN_LEFT, INPUT);
  pinMode(PIN_RIGHT, INPUT);
  pinMode(PIN_STOP, INPUT);

  // 3. 动态配置 I2C 引脚并初始化陀螺仪
  Wire.begin(I2C_SDA, I2C_SCL);
  Wire.setTimeOut(20);
  if (mpu.begin()) {
    Serial.println("#MPU,OK");
    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
    
    // 初始化成功，状态灯短暂亮绿光
    statusLed.setPixelColor(0, statusLed.Color(0, 255, 0)); 
    statusLed.show();
    delay(500); 
  } else {
    // 如果找不到陀螺仪，状态灯爆红，方便排障
    solarOTABootFailure();
    while (1) {
      Serial.println("#MPU,ERROR,begin_failed");
      statusLed.setPixelColor(0, statusLed.Color(255, 0, 0));
      statusLed.show();
      delay(500);
      statusLed.setPixelColor(0, statusLed.Color(0, 0, 0));
      statusLed.show();
      delay(500);
    }
  }
  solarBLEBegin();
}

void loop() {
  unsigned long currentTime = millis();

  // 1. 高速扫描无线遥控指令
  if (digitalRead(PIN_LEFT) == HIGH) {
    if (currentMode != 1) {
      currentMode = 1;
      animationStep = 0;
      isPausing = false;
      lastUpdateTime = currentTime;
    }
  } 
  else if (digitalRead(PIN_RIGHT) == HIGH) {
    if (currentMode != 2) {
      currentMode = 2;
      animationStep = 0;
      isPausing = false;
      lastUpdateTime = currentTime;
    }
  } 
  else if (digitalRead(PIN_STOP) == HIGH) {
    if (currentMode != 0) {
      currentMode = 0;
      animationStep = 0;
    }
  }

  // 2. 数字化双重滤波算法 (刹车检测)
  sensors_event_t a{}, g{}, temp{};
  const bool imuReadOK=readCheckedIMU(a,g,temp);
  const unsigned long debugSampleUs = micros();
  const unsigned long debugLoopDtUs = debugLastSampleUs == 0 ? 0 :
      debugSampleUs - debugLastSampleUs;
  debugLastSampleUs = debugSampleUs;
  debugSequence++;
  float currentY = a.acceleration.y;
  baselineY = baselineY * 0.95 + currentY * 0.05;
  float diff = currentY - baselineY;

  bool oldBraking = false;
  if (abs(diff) > brakeThreshold) {
    hitCounter++;
    if (hitCounter >= requiredHits) {
      oldBraking = true;
    }
  } else {
    hitCounter = 0; 
  }

  if (!imuReadOK) brakeDetector.sensorFailure(debugSampleUs);
  else brakeDetector.update(debugSampleUs,
      {a.acceleration.x,a.acceleration.y,a.acceleration.z},
      {g.gyro.x,g.gyro.y,g.gyro.z});
  const bool wasUpdating=solarOTAMaintenance.load();
  const bool plausible=imuReadOK && Vec3{a.acceleration.x,a.acceleration.y,a.acceleration.z}.norm()>=1
    && Vec3{a.acceleration.x,a.acceleration.y,a.acceleration.z}.norm()<=80
    && Vec3{g.gyro.x,g.gyro.y,g.gyro.z}.norm()<20;
  solarOTABootObserve(plausible,currentTime);
  const bool parked=plausible && brakeDetector.ready && !brakeDetector.braking
    && brakeDetector.accelStdMax<0.06f && brakeDetector.gyroStdMax<0.008f
    && brakeDetector.linear.norm()<0.25f;
  solarOTAPoll(parked,currentTime);
  if(solarOTAMaintenance.load()){
    // Updating is a parked maintenance operation; ride signals are suspended.
    strip.clear();strip.show();
    statusLed.setPixelColor(0,statusLed.Color(0,0,currentTime/250%2?80:15));statusLed.show();
    delay(2);return;
  }
  if(wasUpdating){brakeDetector.restartCalibration();}
  const bool isBraking=brakeDetector.braking;
  static bool previousBraking=false;
  if (isBraking && !previousBraking) {
    flashState=false;
    lastFlashTime=currentTime-50; // Start visibly on the first detected frame.
  }
  previousBraking=isBraking;

  // 3. 状态灯 (Status LED) 智能交互逻辑
  if (currentTime - lastStatusTime >= 250) { // 250ms 刷新一次状态灯
    lastStatusTime = currentTime;
    statusBlink = !statusBlink;
    
    if (!brakeDetector.ready) {
      if (brakeDetector.reason==BrakeDetector::Invalid)
        statusLed.setPixelColor(0,statusLed.Color(50,0,50));
      else statusLed.setPixelColor(0,statusLed.Color(50,50,0));
    } else if (isBraking) {
      // 刹车时：状态灯常亮红色
      statusLed.setPixelColor(0, statusLed.Color(200, 0, 0));
    } else if (currentMode == 1 || currentMode == 2) {
      // 转向时：状态灯跟随闪烁黄色
      if (statusBlink) statusLed.setPixelColor(0, statusLed.Color(200, 100, 0));
      else statusLed.setPixelColor(0, statusLed.Color(0, 0, 0));
    } else {
      // 常规骑行：状态灯显示微弱青色(Cyan)，代表系统健康
      statusLed.setPixelColor(0, statusLed.Color(0, 50, 50));
    }
    statusLed.show();
  }

  // 4. 主大灯带显示逻辑
  if (isBraking) {
    // 刹车最高优先级：红灯高频暴闪
    if (currentTime - lastFlashTime >= 50) {
      lastFlashTime = currentTime;
      flashState = !flashState;
      if (flashState) {
        for(int i=0; i<NUM_PIXELS; i++) strip.setPixelColor(i, strip.Color(255, 0, 0));
      } else {
        for(int i=0; i<NUM_PIXELS; i++) strip.setPixelColor(i, strip.Color(0, 0, 0));
      }
      strip.show();
    }
  } 
  else {
    // 恢复常规主逻辑
    if (currentMode == 0) {
      // 模式0：常规骑行对称流动
      if (currentTime - lastMode0Time >= 80) {
        lastMode0Time = currentTime;
        for (int i = 0; i < NUM_PIXELS; i++) strip.setPixelColor(i, strip.Color(0, 0, 0));
        
        strip.setPixelColor(4 - animationStep, strip.Color(150, 0, 0));
        strip.setPixelColor(5 + animationStep, strip.Color(150, 0, 0));
        strip.show();
        
        animationStep++;
        if (animationStep >= 5) { // 假设灯带对称，共10颗
          animationStep = 0;
        }
      }
    } 
    else if (currentMode == 1) {
      // 模式1：左转流水
      unsigned long requiredDelay = isPausing ? pauseDelay : frameDelay;
      if (currentTime - lastUpdateTime >= requiredDelay) {
        lastUpdateTime = currentTime;
        if (isPausing) {
          isPausing = false;
          animationStep = 0;
        }
        if (animationStep == 0) {
          for (int i = 0; i < NUM_PIXELS; i++) strip.setPixelColor(i, strip.Color(0, 0, 0));
        }
        if (animationStep < NUM_PIXELS) {
          strip.setPixelColor(9 - animationStep, strip.Color(240, 90, 0)); 
          strip.show();
          animationStep++;
        } else {
          isPausing = true;
        }
      }
    } 
    else if (currentMode == 2) {
      // 模式2：右转流水
      unsigned long requiredDelay = isPausing ? pauseDelay : frameDelay;
      if (currentTime - lastUpdateTime >= requiredDelay) {
        lastUpdateTime = currentTime;
        if (isPausing) {
          isPausing = false;
          animationStep = 0;
        }
        if (animationStep == 0) {
          for (int i = 0; i < NUM_PIXELS; i++) strip.setPixelColor(i, strip.Color(0, 0, 0));
        }
        if (animationStep < NUM_PIXELS) {
          strip.setPixelColor(animationStep, strip.Color(240, 90, 0)); 
          strip.show();
          animationStep++;
        } else {
          isPausing = true;
        }
      }
    }
  }
  debugTelemetry(a, g, diff, oldBraking, debugSampleUs, debugLoopDtUs);
}
