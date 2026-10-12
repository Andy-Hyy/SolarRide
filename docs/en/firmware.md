# Firmware

[中文](../zh-CN/firmware.md) · [Home](../../README.md)

Open `firmware/SolarRider_BLE_OTA_V6_5_20261011/SolarRider_BLE_OTA_V6_5_20261011.ino`. Keep the folder name.

Arduino-ESP32 3.3.12, board AirM2M CORE ESP32C3, FQBN `esp32:esp32:AirM2M_CORE_ESP32C3`, 4 MB, default OTA partitions. Serial monitor 115200. Library versions are in [build-profile.json](../../firmware/build-profile.json).

Copy `OTAPairing.example.h` to `OTAPairing.h`, set a six-digit PIN, delete the `#error`. Do not commit `OTAPairing.h`.

From the repository root:

```sh
arduino-cli core update-index --additional-urls https://espressif.github.io/arduino-esp32/package_esp32_index.json
arduino-cli core install esp32:esp32@3.3.12 --additional-urls https://espressif.github.io/arduino-esp32/package_esp32_index.json
arduino-cli lib update-index
arduino-cli lib install "Adafruit NeoPixel@1.15.5" "Adafruit MPU6050@2.2.9" "Adafruit BusIO@1.17.4" "Adafruit Unified Sensor@1.1.15"
```

I upload from the Arduino IDE. A BLE update sends the `.ino.bin` only. Each application slot is `0x140000` bytes.
