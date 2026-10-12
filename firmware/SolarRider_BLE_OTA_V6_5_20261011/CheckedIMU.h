#pragma once
#include <Adafruit_Sensor.h>
#include <Wire.h>

// The installed Adafruit getEvent() always returns true even if its internal
// read failed. Read the known 8g/500dps register block with an explicit check.
static bool readCheckedIMU(sensors_event_t &a,sensors_event_t &g,sensors_event_t &temp){
  constexpr uint8_t address=0x68;
  Wire.beginTransmission(address);Wire.write(0x3B);
  if(Wire.endTransmission(false)!=0)return false;
  if(Wire.requestFrom(address,(uint8_t)14,(uint8_t)true)!=14){
    while(Wire.available())Wire.read();return false;
  }
  uint8_t bytes[14];for(unsigned i=0;i<14;++i)bytes[i]=(uint8_t)Wire.read();
  // Avoid Arduino word(...) macro: decode the sensor bytes, not the index.
  auto signed16At=[&](unsigned i){return (int16_t)(((uint16_t)bytes[i]<<8)|bytes[i+1]);};
  const float accelScale=SENSORS_GRAVITY_STANDARD/4096.0f;
  const float gyroScale=(3.14159265358979323846f/180.0f)/65.5f;
  a.acceleration.x=signed16At(0)*accelScale;a.acceleration.y=signed16At(2)*accelScale;a.acceleration.z=signed16At(4)*accelScale;
  temp.temperature=signed16At(6)/340.0f+36.53f;
  g.gyro.x=signed16At(8)*gyroScale;g.gyro.y=signed16At(10)*gyroScale;g.gyro.z=signed16At(12)*gyroScale;
  return true;
}
