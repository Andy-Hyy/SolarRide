# Mounting and direction calibration

[中文](../zh-CN/calibration.md) · [Home](../../README.md)


## Two different references

At startup, the firmware learns a stationary gravity reference and gyro bias. It does **not** automatically learn which way the bicycle points. `BrakeDetector.h` contains a fixed forward vector measured for the existing installation:

```cpp
const Vec3 forward={-0.5883380f,-0.7899954f,0.1725274f};
```

Do not copy this vector blindly to a differently oriented MPU6050. The included [mount data](../../tests/fixtures/mount_direction_calibration.json) is a build-specific example, not a universal calibration.

## Obtain your direction

1. Fasten the IMU and bag in their final positions. Keep the bicycle upright and stationary; record a stable average acceleration vector `g0` over several seconds.
2. In a controlled stationary setup, raise the front wheel modestly, keep the bicycle upright, and record another stable average `g1`. Lower it and verify the original reference returns. Support the bicycle securely.
3. Form `v = g1/|g1|`. Remove its component along the original gravity direction: `p = v - g0 * dot(v,g0)/dot(g0,g0)`. Normalize `p` to obtain a candidate forward axis. A very small `|p|` means the measurement is too noisy or the lift is insufficient.
4. Put the candidate vector in `BrakeDetector.h`, rebuild and upload over USB. During a clearly labeled forward push and stop, verify that braking gives **negative** `forward_filtered`, below the negative threshold. If the sign is wrong, reverse the vector and repeat the check.
5. Confirm stationary behavior, several gentle and firmer push/stops, and return to a released brake state. Save your mounting and calibration notes.

At each power-on, keep the bicycle upright and still for roughly two seconds, then wait for the cyan status pixel / `ready=1`. Motion can delay calibration; the detector waits for a suitable stationary window. Recalibrate the forward vector after a meaningful sensor reorientation. Small-placement host tests do not establish performance for a loosely swinging bag or arbitrary mounting.
