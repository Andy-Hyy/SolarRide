# 固件

[English](../en/firmware.md) · [首页](../../README.zh-CN.md)

打开 `firmware/SolarRider_BLE_OTA_V6_5_20261011/SolarRider_BLE_OTA_V6_5_20261011.ino`。目录名必须保持不变。

已核对的工具链为 Arduino-ESP32 3.3.12，开发板 AirM2M CORE ESP32C3，FQBN `esp32:esp32:AirM2M_CORE_ESP32C3`，闪存 4 MB，分区方案为默认 OTA。串口监视器波特率为 115200。库版本见 [build-profile.json](../../firmware/build-profile.json)。

将 `OTAPairing.example.h` 复制为 `OTAPairing.h`，填入六位配对码，并删除 `#error`。请勿提交 `OTAPairing.h`。

在仓库根目录执行：

```sh
arduino-cli core update-index --additional-urls https://espressif.github.io/arduino-esp32/package_esp32_index.json
arduino-cli core install esp32:esp32@3.3.12 --additional-urls https://espressif.github.io/arduino-esp32/package_esp32_index.json
arduino-cli lib update-index
arduino-cli lib install "Adafruit NeoPixel@1.15.5" "Adafruit MPU6050@2.2.9" "Adafruit BusIO@1.17.4" "Adafruit Unified Sensor@1.1.15"
```

上传通过 Arduino IDE 完成。蓝牙升级仅发送 `.ino.bin` 应用镜像。每个应用分区为 `0x140000` 字节。
