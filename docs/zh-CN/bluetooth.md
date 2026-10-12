# 蓝牙数据与维护

[English](../en/bluetooth.md) · [首页](../../README.zh-CN.md)


## 只读遥测

设备广播名为 `SolarRider`。服务 `6E400001-B5A3-F393-E0A9-E50E24DCCA9E` 的通知特征为 `6E400003-B5A3-F393-E0A9-E50E24DCCA9E`。CSV 行按 20 字节分包发送，客户端需拼回以换行结尾的完整行。BLE 最多约 10 行/秒，可能丢弃中间行；USB 目标最多约 50 行/秒。两者都不是车载持久日志。

Mac 如缺少编译工具，先安装 Xcode Command Line Tools，再运行：

```sh
bash tools/macos/build_tools.sh
mkdir -p logs
tools/macos/ble_capture 300 > logs/capture.txt
```

秒数参数包含连接耗时，系统提示时允许蓝牙访问，并关闭其他已连接客户端。工具会在断开后重新扫描，忽略不完整行；它选择第一个匹配的 `SolarRider`，使用时附近只给目标设备通电。

V6.5 一行有 26 列：`t_us,seq,loop_dt_us,ax,ay,az,gx,gy,gz,gravity_x,gravity_y,gravity_z,forward_raw,forward_filtered,old_diff_y,old_hits,old_brake,ready,brake,events,mode,dropped,cal_reason,acc_std_max,gyro_std_max,gravity_error`。加速度单位为 m/s²，角速度为 rad/s。校准原因码为 0 就绪、1 收集、2 加速度晃动、3 角速度晃动、4 无效/读取失败、5 间断、6 幅度异常。`dropped` 统计写不进 USB 调试缓冲的行数，不是完整的 BLE 丢包计数。

## 蓝牙升级

运行 `tools/macos/ble_update`，选择自己编译的应用 `.bin`。设备停车静止，完成校准，关闭数据读取工具，开机后约等八秒再尝试升级。提示配对时输入自己本地头文件中的私有码。Mac 工具采用交互式文件选择和中文提示。

可先离线核验镜像：

```sh
tools/macos/ble_update --validate build/firmware/SolarRider_BLE_OTA_V6_5_20261011.ino.bin
```

OTA 服务 `0x8018` 使用 firmware `0x8020`、progress `0x8021`、command `0x8022` 和 information `0x8023`。传输按 4 KiB 分段，检查 CRC16 和顺序，接收端核验 C3 应用身份、大小和 OTA 标记。CRC 和镜像哈希用于完整性核验，不是发布者数字签名。

工具会等待重启并检查镜像身份和启动状态。当前提供接收实现和主机检查，但实际 BLE 完整传输、重启和恢复尚未验证。通用文件导入或协议兼容不等于 iPhone 实测兼容。后续版本需保留 `SolarRider_OTA_PROTOCOL_1` 和 OTA 接收实现。回滚接口依赖核心及引导程序配置，不保证一定恢复，仍需保留 USB 接口。
