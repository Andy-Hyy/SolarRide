# How the design works

[中文](../zh-CN/design.md) · [Home](../../README.md)


## Control path

The handlebar remote talks to a 433 MHz receiver. The ESP32-C3 reads the receiver's left, right, and stop pins. BLE is a second link, for live data and updates.

The main strip contains 10 pixels. In normal mode, two red pixels move outward from the center. Left and right commands select opposite amber fill directions. Stop returns to normal mode. A detected braking state overrides the strip temporarily, after which the previously selected mode resumes.

## Braking indication

An MPU6050 provides acceleration and angular velocity. The firmware obtains a stationary gravity reference and gyro bias, propagates gravity during rotation, and projects gravity-corrected acceleration onto a fixed mounting-specific forward vector. A first-order 35 ms filter feeds a -0.70 m/s² threshold with 150 ms continuous confirmation. The indication is held for at least 700 ms after confirmation, and can remain active while deceleration continues.

The `old_*` columns are an older detector I left in the log so I could compare. The lights follow the newer detector.

## Power and packaging

The panels and the USB-C board charge the cell. The [power guide](power-wiring.md) is the CN3163, 4056-TP, and XR2981 wiring I used. How much the panels add depends on light, shade, and angle. I have not measured that.

The board holds the modules and the connectors. The box holds that inside the bag. The sensor stays in one orientation, because the forward vector in the sketch is for this mounting.
