# Direction

[中文](../zh-CN/calibration.md) · [Home](../../README.md)

At startup the firmware estimates gravity and gyro bias. It does not estimate the bicycle heading. The forward vector in `BrakeDetector.h` matches the enclosure mounting on this bicycle:

```cpp
const Vec3 forward={-0.5883380f,-0.7899954f,0.1725274f};
```

The brake warning uses a 35 ms filter. It asserts when forward acceleration stays below −0.70 m/s² for 150 ms, then holds for at least 700 ms.

A different mounting requires a new vector. Record `g0` with the bicycle upright and stationary. Raise the front wheel slightly and record `g1`. Set `v = g1/|g1|`, then `p = v - g0 * dot(v,g0)/dot(g0,g0)`, and normalize `p`. After upload, a forward stop should make `forward_filtered` negative. If the sign is reversed, invert the vector.

At power-on, keep the bicycle upright and stationary for about two seconds, until the status pixel turns cyan (`ready=1`).
