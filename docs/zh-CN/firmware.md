# 固件

[English](../en/firmware.md) · [首页](../../README.md)

打开 `firmware/SolarRider_BLE_OTA_V6_5_20261011/SolarRider_BLE_OTA_V6_5_20261011.ino`。目录名别改。

Arduino-ESP32 3.3.12，板子 AirM2M CORE ESP32C3，FQBN `esp32:esp32:AirM2M_CORE_ESP32C3`，4 MB，默认 OTA 分区。串口 115200。库版本在 [build-profile.json](../../firmware/build-profile.json)。

把 `OTAPairing.example.h` 复制成 `OTAPairing.h`，填六位配对码，删掉 `#error`。不要提交 `OTAPairing.h`。

在仓库根目录：

```sh
arduino-cli core update-index --additional-urls https://espressif.github.io/arduino-esp32/package_esp32_index.json
arduino-cli core install esp32:esp32@3.3.12 --additional-urls https://espressif.github.io/arduino-esp32/package_esp32_index.json
arduino-cli lib update-index
arduino-cli lib install "Adafruit NeoPixel@1.15.5" "Adafruit MPU6050@2.2.9" "Adafruit BusIO@1.17.4" "Adafruit Unified Sensor@1.1.15"
```

我用 Arduino IDE 上传。蓝牙升级只发 `.ino.bin`。每个应用分区 `0x140000` 字节。
