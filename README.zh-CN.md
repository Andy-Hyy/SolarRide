# SolarRide

![车上](docs/assets/bicycle-mounted.jpg)

ESP32-C3 装在车包上。433 MHz 遥控器管转向灯。MPU6050 做刹车提示。两块 1 W/6 V 太阳能板进 CN3163，USB-C 接 4056-TP，XR2981 升到 5 V。电池是 103040，1200 mAh。

程序在 `firmware/SolarRider_BLE_OTA_V6_5_20261011/`。这个目录名别改。蓝牙名是 `SolarRider`。把 `OTAPairing.example.h` 复制成 `OTAPairing.h`，填六位配对码，删掉 `#error`。不要提交 `OTAPairing.h`。

Arduino-ESP32 3.3.12，板子 AirM2M CORE ESP32C3，4 MB，OTA 分区。

2026-10-11 查过：USB 烧录，哈希对得上。遥控左、右、停止。推车再停，刹车灯亮。盒子装上车、按那个倾斜改过阈值之后，减速时刹车提示会亮。一段记录大约 180 秒、8072 行，没有读数错误，四次刹车事件，都释放了。

没量：电池能用多久，太阳能实际充了多少，别的路上会不会误闪，完整的蓝牙升级和恢复。

[接线](docs/zh-CN/hardware.md) · [English](README.md)
