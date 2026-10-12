# 固件编译与烧录

[English](../en/firmware.md) · [首页](../../README.md)


## 源码与工具链

打开 `firmware/SolarRider_BLE_OTA_V6_5_20261011/SolarRider_BLE_OTA_V6_5_20261011.ino`。保留目录名，方便 Arduino 正确认出程序。

我编译用的是 Arduino-ESP32 **3.3.12**、开发板 **AirM2M CORE ESP32C3**、FQBN `esp32:esp32:AirM2M_CORE_ESP32C3`、4 MB 闪存，分区用默认的 OTA。安装说明在 [Espressif 的页面](https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html)。库版本在 [build-profile.json](../../firmware/build-profile.json)。BLE、Wire、Update 跟核心一起装。

## 设置私有配对码

把 `OTAPairing.example.h` 复制成同目录下的 `OTAPairing.h`，填上自己的六位配对码，删掉 `#error` 行。不要提交 `OTAPairing.h`。示例在改完之前编译不过。改配对码要重新编译并烧录，不会改已经烧进板子里的程序。

## 命令行编译

安装 Arduino CLI 后，在仓库根目录运行：

```sh
arduino-cli core update-index --additional-urls https://espressif.github.io/arduino-esp32/package_esp32_index.json
arduino-cli core install esp32:esp32@3.3.12 --additional-urls https://espressif.github.io/arduino-esp32/package_esp32_index.json
arduino-cli lib update-index
arduino-cli lib install "Adafruit NeoPixel@1.15.5" "Adafruit MPU6050@2.2.9" "Adafruit BusIO@1.17.4" "Adafruit Unified Sensor@1.1.15"
```

我是用 Arduino IDE 上传的。编译前先复制配对码头文件。上面几条是装核心和库。

## 首次 USB 烧录

在 Arduino IDE 里选这块板、选板子插上的那个口，再上传。串口监视器是 **115200**。USB 口留着。别的 ESP32-C3 板子引脚可能不一样。

默认分区是两个各 `0x140000` 字节的应用区，外加 OTA 数据。要无线更新就留 OTA 分区。发出去的是编译出来的 `.ino.bin`，不是 merged、bootloader 或 partitions 那个文件。

## 源码文件

| 文件 | 作用 |
| --- | --- |
| `.ino` | 引脚、遥控模式、灯效、遥测与主循环 |
| `BrakeDetector.h` | 重力/偏置估计与减速提示 |
| `CheckedIMU.h` | 在 0x68 检查读取 MPU6050 的 14 字节数据 |
| `SolarRiderBLE.h` | BLE 遥测队列与通知 |
| `SolarRiderOTA.h` | 蓝牙升级，以及启动后确认新程序 |
| `EspBleOTAProtocol.h` | 升级包的顺序、分段 CRC |
| `OTAPairing.example.h` | 复制一份，再填自己的配对码 |
