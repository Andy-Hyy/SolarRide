# SolarRide

![装车照片](docs/assets/bicycle-mounted.jpg)

SolarRide 是装在车包上的自行车灯控，控制器为 ESP32-C3。车把上的 433 MHz 遥控器用于切换转向灯，MPU6050 在减速时提供刹车提示。两块 1 W/6 V 太阳能板并联接入 CN3163。USB-C 充电由 4056-TP 完成，XR2981 将供电升至 5 V。电池为 103040，容量 1200 mAh。

固件位于 `firmware/SolarRider_BLE_OTA_V6_5_20261011/`。目录名必须保持不变，否则 Arduino 无法识别该工程。蓝牙广播名称为 `SolarRider`。编译前将 `OTAPairing.example.h` 复制为 `OTAPairing.h`，填入六位配对码，并删除 `#error`。请勿提交 `OTAPairing.h`。

已核对的工具链为 Arduino-ESP32 3.3.12，开发板 AirM2M CORE ESP32C3，闪存 4 MB，分区方案为默认 OTA。

2026-10-11 已核对：USB 烧录完成，镜像哈希一致。遥控器的左转、右转和停止均有响应。推车后刹停时，刹车灯点亮。控制盒装车后，已按该倾斜调整阈值，减速时刹车提示可以触发。一次采集约 180 秒，共 8072 行，无读取错误。其中四次刹车事件均已释放。

尚未测量：电池续航、太阳能充电电流、其他路面上的误触发，以及完整的蓝牙固件升级与恢复。

[接线](docs/zh-CN/hardware.md) · [English](README.md)
