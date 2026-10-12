# Direction

[中文](../zh-CN/calibration.md) · [Home](../../README.md)

Startup learns gravity and gyro bias. It does not learn which way the bike points. The forward vector in `BrakeDetector.h` is the box on my bike:

```cpp
const Vec3 forward={-0.5883380f,-0.7899954f,0.1725274f};
```

Brake light: 35 ms filter, below −0.70 m/s² for 150 ms, then hold at least 700 ms.

Another mounting needs a new vector. Record `g0` with the bike upright and still. Lift the front wheel a little and record `g1`. `v = g1/|g1|`, then `p = v - g0 * dot(v,g0)/dot(g0,g0)`, and normalize `p`. After upload, a forward stop should make `forward_filtered` negative. If the sign is wrong, flip the vector.

Power-on: bike upright and still for about two seconds, then the status pixel is cyan (`ready=1`).
