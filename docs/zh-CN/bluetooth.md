# 蓝牙数据与维护

[English](../en/bluetooth.md) · [首页](../../README.md)


## 只读遥测

设备广播名为 `SolarRider`。服务 `6E400001-B5A3-F393-E0A9-E50E24DCCA9E` 的通知特征为 `6E400003-B5A3-F393-E0A9-E50E24DCCA9E`。CSV 行按 20 字节分包发送，客户端需拼回以换行结尾的完整行。BLE 最多约 10 行/秒，可能丢弃中间行；USB 目标最多约 50 行/秒。两者都不是车载持久日志。

我在自己的 Mac 上用一个小程序读这些数据，那个程序没有放进这个仓库。它会连上第一个名叫 `SolarRider` 的设备，所以旁边只给我要读的那台盒子通电。先关掉其他已经连上的客户端。一行数据可能会拆开发过来。

V6.5 一行有 26 列：`t_us,seq,loop_dt_us,ax,ay,az,gx,gy,gz,gravity_x,gravity_y,gravity_z,forward_raw,forward_filtered,old_diff_y,old_hits,old_brake,ready,brake,events,mode,dropped,cal_reason,acc_std_max,gyro_std_max,gravity_error`。加速度单位为 m/s²，角速度为 rad/s。校准原因码为 0 就绪、1 收集、2 加速度晃动、3 角速度晃动、4 无效/读取失败、5 间断、6 幅度异常。`dropped` 统计写不进 USB 调试缓冲的行数，不是完整的 BLE 丢包计数。

## 蓝牙升级

手机端的升级程序也只在我自己电脑上，没有放进这个仓库。如果要试升级，先把车停稳，等盒子校准完成，配对码用你自己 `OTAPairing.h` 里的。我还没有在真车上走完一次无线升级、重启和恢复。USB 口要留着。

OTA 服务 `0x8018` 使用 firmware `0x8020`、progress `0x8021`、command `0x8022` 和 information `0x8023`。传输按 4 KiB 分段，检查 CRC16 和顺序，接收端核验 C3 应用身份、大小和 OTA 标记。CRC 和镜像哈希只用来看数据有没有坏，不是签名。以后的版本要留下 `SolarRider_OTA_PROTOCOL_1` 和程序里的接收部分。回滚要看引导程序，不保证坏掉的升级一定能自己恢复。
