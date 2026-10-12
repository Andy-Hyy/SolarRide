# 固件编译与烧录

[English](../en/firmware.md) · [首页](../../README.zh-CN.md)


## 源码与工具链

打开 `firmware/SolarRider_BLE_OTA_V6_5_20261011/SolarRider_BLE_OTA_V6_5_20261011.ino`。保留目录名，方便 Arduino 正确认出程序。

已核验配置为 Arduino-ESP32 **3.3.12**、开发板 **AirM2M CORE ESP32C3**、FQBN `esp32:esp32:AirM2M_CORE_ESP32C3`、4 MB 闪存和默认支持 OTA 的分区。安装方法见 [Espressif 官方说明](https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html)。库版本见 [build-profile.json](../../firmware/build-profile.json)。BLE、Wire、Update 随核心提供，库管理器可能额外安装 Adafruit 声明的依赖。

## 设置私有配对码

在仓库根目录运行 `python3 tools/configure_pairing.py`，它会在程序旁生成被忽略的本地 `OTAPairing.h`。配对时自行查看此文件。也可用 `--pin` 指定六位数字；确需替换已有配置时才用 `--force`。不要提交这个文件。

不使用配置工具时，将 `OTAPairing.example.h` 复制为 `OTAPairing.h`，设置六位私有码并删除 `#error` 行。公开示例刻意要求配置后才能编译。修改配对码需要重新编译和烧录，运行工具不会改变已烧录的设备。

## 命令行编译

安装 Arduino CLI 后，在仓库根目录运行：

```sh
arduino-cli core update-index --additional-urls https://espressif.github.io/arduino-esp32/package_esp32_index.json
arduino-cli core install esp32:esp32@3.3.12 --additional-urls https://espressif.github.io/arduino-esp32/package_esp32_index.json
arduino-cli lib update-index
arduino-cli lib install "Adafruit NeoPixel@1.15.5" "Adafruit MPU6050@2.2.9" "Adafruit BusIO@1.17.4" "Adafruit Unified Sensor@1.1.15"
python3 tools/configure_pairing.py
bash tools/build_firmware.sh
```

编译脚本只编译，输出位于被忽略的 `build/firmware/`，不向设备烧录。配置工具不会覆盖已有私有码；如已配置，跳过该行。

## 首次 USB 烧录

在 Arduino IDE 中选择对应开发板和配置，选择实际连接的端口，再上传。串口监视器使用 **115200 波特率**。尝试自己的固件时保留 USB 恢复条件。更换开发板、分区或库版本需重新核验，不能默认所有 ESP32-C3 板的引脚布局相同。

默认分区包含两个各 `0x140000` 字节的应用分区及 OTA 元数据。需要无线更新时不要选 No OTA 分区。后续蓝牙升级只选择编译生成的 `.ino.bin` **应用镜像**，不要选 merged、bootloader 或 partitions 镜像。

## 源码文件

| 文件 | 作用 |
| --- | --- |
| `.ino` | 引脚、遥控模式、灯效、遥测与主循环 |
| `BrakeDetector.h` | 重力/偏置估计与减速提示 |
| `CheckedIMU.h` | 在 0x68 检查读取 MPU6050 的 14 字节数据 |
| `SolarRiderBLE.h` | BLE 遥测队列与通知 |
| `SolarRiderOTA.h` | 蓝牙 OTA、镜像核验与启动健康确认接口 |
| `EspBleOTAProtocol.h` | OTA 顺序、分段 CRC 与提交状态机 |
| `OTAPairing.example.h` | 公开模板，用于生成本地私有配置 |

原有固件标识中的 `TRIAL_V6_5` 保留用于工具兼容，不代表额外的成熟度结论。公开源码保留现用 V6.5 的操作逻辑，配对码另行设置。新编译镜像有自己的哈希，不等同于此前烧录的镜像。
