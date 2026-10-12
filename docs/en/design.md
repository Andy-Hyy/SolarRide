# How the design works

[中文](../zh-CN/design.md) · [Home](../../README.md)


## Control path

The handlebar transmitter talks to a 433 MHz receiver. Its logic outputs are read as left, right and stop inputs by the ESP32-C3. The receiver handles the RF link; the firmware does not decode raw 433 MHz packets. BLE is a separate link for live data and maintenance, not for remote turn control.

The main strip contains 10 pixels. In normal mode, two red pixels move outward from the center. Left and right commands select opposite amber fill directions. Stop returns to normal mode. A detected braking state overrides the strip temporarily, after which the previously selected mode resumes.

## Braking indication

An MPU6050 provides acceleration and angular velocity. The firmware obtains a stationary gravity reference and gyro bias, propagates gravity during rotation, and projects gravity-corrected acceleration onto a fixed mounting-specific forward vector. A first-order 35 ms filter feeds a -0.70 m/s² threshold with 150 ms continuous confirmation. The indication is held for at least 700 ms after confirmation, and can remain active while deceleration continues.

`old_*` telemetry columns retain an earlier detector for comparison; they do not control the lamps. Firmware events count detector transitions, not verified physical braking actions.

## Power and packaging

Solar-assisted charging and a battery supply feed the electronics through the selected charging/protection/regulation modules. The [power guide](power-wiring.md) shows the confirmed CN3163 / 4056-TP / XR2981 connections. Solar output depends on illumination, shading and panel angle; no energy balance or runtime guarantee is made.

The carrier board organizes purchased modules and external connectors. The enclosure protects and locates the electronics inside a bicycle bag. Fixing the IMU in a repeatable position matters because the firmware's forward vector is specific to an installation.
