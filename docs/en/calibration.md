# Mounting and direction calibration

[中文](../zh-CN/calibration.md) · [Home](../../README.md)


## Two different references

At startup, the firmware learns a stationary gravity reference and gyro bias. It does **not** automatically learn which way the bicycle points. `BrakeDetector.h` contains a fixed forward vector measured for the existing installation:

```cpp
const Vec3 forward={-0.5883380f,-0.7899954f,0.1725274f};
```

That vector is the direction of the box on my bike. If the MPU6050 faces another way, measure a new one.

## Obtain your direction

1. Fasten the IMU and bag in their final positions. Keep the bicycle upright and stationary; record a stable average acceleration vector `g0` over several seconds.
2. With the bike held upright, lift the front wheel a little and record another stable average, `g1`. Put the wheel down and check that the first reading comes back.
3. Form `v = g1/|g1|`. Remove its component along the original gravity direction: `p = v - g0 * dot(v,g0)/dot(g0,g0)`. Normalize `p` to obtain a candidate forward axis. A very small `|p|` means the measurement is too noisy or the lift is insufficient.
4. Put that vector in `BrakeDetector.h`, rebuild, and upload over USB. Push the bike forward and stop. Braking should make `forward_filtered` **negative**, below the negative threshold. If the sign is wrong, flip the vector and try again.
5. Then I try it sitting still, a few gentle stops, a few harder ones, and I watch the warning turn off again.

Each time I turn it on, I hold the bike upright and still for about two seconds, then wait for the cyan pixel or `ready=1`. If the bike is moving, calibration waits. If I turn the sensor, I measure the forward vector again.
