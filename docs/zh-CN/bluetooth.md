# 蓝牙数据与维护

[English](../en/bluetooth.md) · [首页](../../README.md)


## 只读遥测

设备广播名为 `SolarRider`。服务 `6E400001-B5A3-F393-E0A9-E50E24DCCA9E` 的通知特征为 `6E400003-B5A3-F393-E0A9-E50E24DCCA9E`。每一行是 ASCII CSV，拆成 20 字节发出去。读的一边要在换行处拼回去。BLE 大约最多 10 行/秒，中间的行可能丢掉。USB 大约最多 50 行/秒。盒子自己不存这些行。

我在自己的 Mac 上用一个小程序读这些数据，那个程序没有放进这个仓库。它会连上第一个名叫 `SolarRider` 的设备，所以旁边只给我要读的那台盒子通电。先关掉其他已经连上的客户端。一行数据可能会拆开发过来。

V6.5 一行有 26 列：`t_us,seq,loop_dt_us,ax,ay,az,gx,gy,gz,gravity_x,gravity_y,gravity_z,forward_raw,forward_filtered,old_diff_y,old_hits,old_brake,ready,brake,events,mode,dropped,cal_reason,acc_std_max,gyro_std_max,gravity_error`。加速度单位为 m/s²，角速度为 rad/s。校准原因码为 0 就绪、1 收集、2 加速度晃动、3 角速度晃动、4 无效/读取失败、5 间断、6 幅度异常。`dropped` 是写不进 USB 调试缓冲的行数。

## 蓝牙升级

手机端的升级程序也只在我自己电脑上，没有放进这个仓库。如果要试升级，先把车停稳，等盒子校准完成，配对码用你自己 `OTAPairing.h` 里的。我还没有在真车上走完一次无线升级、重启和恢复。USB 口要留着。

OTA 服务 `0x8018` 使用 firmware `0x8020`、progress `0x8021`、command `0x8022` 和 information `0x8023`。传输按 4 KiB 分段，查 CRC16 和顺序。板子会看镜像是不是 ESP32-C3 的应用程序、大小放不放得下、OTA 标记在不在。CRC16 和镜像哈希用来看字节有没有传全。以后的版本要留下 `SolarRider_OTA_PROTOCOL_1` 和程序里的接收部分。坏镜像会不会退回去，看引导程序。失败的升级我还没在车上试过。
