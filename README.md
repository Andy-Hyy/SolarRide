# SolarRide

![Mounted on the bicycle](docs/assets/bicycle-mounted.jpg)

SolarRide is a bag-mounted bicycle light controller built around an ESP32-C3. A 433 MHz handlebar remote selects the turn signals, and an MPU6050 provides the brake warning during deceleration. Two 1 W/6 V solar panels connect in parallel to a CN3163. USB-C charging is handled by a 4056-TP, and an XR2981 boosts the supply to 5 V. The cell is a 103040, rated 1200 mAh.

The sketch is in `firmware/SolarRider_BLE_OTA_V6_5_20261011/`. The folder name must remain unchanged, or Arduino will not recognize the sketch. The BLE advertisement name is `SolarRider`. Before building, copy `OTAPairing.example.h` to `OTAPairing.h`, enter a six-digit PIN, and remove the `#error` line. Do not commit `OTAPairing.h`.

The checked toolchain is Arduino-ESP32 3.3.12, board AirM2M CORE ESP32C3, 4 MB flash, and the default OTA partition scheme.

Verified on 2026-10-11: the USB upload completed and the image hash matched. Remote left, right, and stop all responded. The brake lamps lit when the bicycle was pushed and then stopped. After the enclosure was mounted, the threshold was adjusted for that tilt, and the brake warning then responded to deceleration. One capture ran for about 180 s and contained 8072 rows, with no read errors. All four brake events in that capture returned to the released state.

Not yet measured: battery runtime, solar charging current, false triggers on other roads, and a complete BLE firmware update followed by recovery.

[Wiring](docs/en/hardware.md) · [中文](README.zh-CN.md)
