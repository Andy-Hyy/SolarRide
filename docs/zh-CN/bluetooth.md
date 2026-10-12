# 蓝牙

[English](../en/bluetooth.md) · [首页](../../README.zh-CN.md)

广播名称为 `SolarRider`。

通知服务为 `6E400001-B5A3-F393-E0A9-E50E24DCCA9E`，特征为 `6E400003-B5A3-F393-E0A9-E50E24DCCA9E`。数据为 CSV 行，按 20 字节分包，在换行处重组。BLE 最高约 10 行/秒，USB 最高约 50 行/秒。

`t_us,seq,loop_dt_us,ax,ay,az,gx,gy,gz,gravity_x,gravity_y,gravity_z,forward_raw,forward_filtered,old_diff_y,old_hits,old_brake,ready,brake,events,mode,dropped,cal_reason,acc_std_max,gyro_std_max,gravity_error`

加速度单位为 m/s²，角速度单位为 rad/s。`cal_reason`：0 就绪，1 采集中，2 加速度，3 角速度，4 读取失败，5 间断，6 幅度无效。`old_*` 为先前的检测结果。灯效由 `brake` 决定。`dropped` 统计未能写入 USB 调试缓冲的行数。

OTA 服务 `0x8018`：firmware `0x8020`，progress `0x8021`，command `0x8022`，information `0x8023`。传输按 4 KiB 分段，使用 CRC16，并按顺序发送。镜像须为带 OTA 标记的 ESP32-C3 应用程序。协议标识为 `SolarRider_OTA_PROTOCOL_1`。仅发送 `.ino.bin`。自行车须处于静止状态。配对码为 `OTAPairing.h` 中的数值。
