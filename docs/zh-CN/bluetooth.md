# 蓝牙

[English](../en/bluetooth.md) · [首页](../../README.md)

广播名 `SolarRider`。

通知服务 `6E400001-B5A3-F393-E0A9-E50E24DCCA9E`，特征 `6E400003-B5A3-F393-E0A9-E50E24DCCA9E`。CSV 行，每包 20 字节，在换行处拼回。BLE 最多约 10 行/秒，USB 最多约 50 行/秒。

`t_us,seq,loop_dt_us,ax,ay,az,gx,gy,gz,gravity_x,gravity_y,gravity_z,forward_raw,forward_filtered,old_diff_y,old_hits,old_brake,ready,brake,events,mode,dropped,cal_reason,acc_std_max,gyro_std_max,gravity_error`

加速度 m/s²，角速度 rad/s。`cal_reason`：0 就绪，1 收集，2 加速度，3 角速度，4 读失败，5 间断，6 幅度不对。`old_*` 是旧算法。灯跟 `brake`。`dropped` 是写不进 USB 调试缓冲的行数。

OTA 服务 `0x8018`：firmware `0x8020`，progress `0x8021`，command `0x8022`，information `0x8023`。4 KiB 一段，CRC16，按顺序。镜像要是带 OTA 标记的 ESP32-C3 应用程序。协议号 `SolarRider_OTA_PROTOCOL_1`。只发 `.ino.bin`。车停着。配对码是 `OTAPairing.h` 里的。
