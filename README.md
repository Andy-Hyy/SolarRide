# SolarRide

![On the bike](docs/assets/bicycle-mounted.jpg)

ESP32-C3 on the bag. 433 MHz remote for the turn lights. MPU6050 brake warning. Two 1 W/6 V panels into a CN3163, USB-C on a 4056-TP, XR2981 boost to 5 V. Cell is a 103040, 1200 mAh.

Sketch: `firmware/SolarRider_BLE_OTA_V6_5_20261011/`. Keep that folder name. BLE name is `SolarRider`. Copy `OTAPairing.example.h` to `OTAPairing.h`, set a six-digit PIN, delete the `#error`. Do not commit `OTAPairing.h`.

Arduino-ESP32 3.3.12, board AirM2M CORE ESP32C3, 4 MB, OTA partitions.

Checked on 2026-10-11: USB upload, hash matched. Remote left, right, and stop. Brake flash on a push-and-stop. After the box was mounted, the threshold was changed for that tilt, then the brake warning came on. One log, about 180 s, 8072 rows, no read errors, four brake events, all released.

Not measured: battery runtime, solar charge current, false flashes on other roads, a full BLE update and recovery.

[Wiring](docs/en/hardware.md) · [中文](README.zh-CN.md)
