# Build and upload firmware

[中文](../zh-CN/firmware.md) · [Home](../../README.md)


## Source and toolchain

Open `firmware/SolarRider_BLE_OTA_V6_5_20261011/SolarRider_BLE_OTA_V6_5_20261011.ino`. Keep its folder name intact so Arduino recognizes the sketch.

I compile with Arduino-ESP32 **3.3.12**, board **AirM2M CORE ESP32C3**, FQBN `esp32:esp32:AirM2M_CORE_ESP32C3`, 4 MB flash, and the default OTA partitions. Install notes are on [Espressif's page](https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html). Library versions are in [build-profile.json](../../firmware/build-profile.json). BLE, Wire, and Update come with the core.

## Configure your PIN

Copy `OTAPairing.example.h` to `OTAPairing.h` in the same folder. Put in your own six-digit PIN and delete the `#error` line. Do not commit `OTAPairing.h`. The example does not build until you do this. Changing the PIN means compiling and uploading again. It does not change a board that already has the old code.

## Command-line build

Install Arduino CLI and run these commands from the repository root:

```sh
arduino-cli core update-index --additional-urls https://espressif.github.io/arduino-esp32/package_esp32_index.json
arduino-cli core install esp32:esp32@3.3.12 --additional-urls https://espressif.github.io/arduino-esp32/package_esp32_index.json
arduino-cli lib update-index
arduino-cli lib install "Adafruit NeoPixel@1.15.5" "Adafruit MPU6050@2.2.9" "Adafruit BusIO@1.17.4" "Adafruit Unified Sensor@1.1.15"
```

I upload from the Arduino IDE. Copy the pairing header before compiling. The commands above install the core and the libraries.

## First USB upload

In the Arduino IDE I pick that board, the port the board is on, and upload. The serial monitor is **115200 baud**. I leave the USB port reachable. Other ESP32-C3 boards can use different pins.

The default partition table has two application slots of `0x140000` bytes each, plus OTA data. A wireless update needs an OTA partition scheme. The file to send is the `.ino.bin` application image, not the merged, bootloader, or partitions file.

## Source layout

| File | Role |
| --- | --- |
| `.ino` | GPIO, remote modes, lamps, telemetry and main loop |
| `BrakeDetector.h` | Gravity/bias estimation and deceleration indication |
| `CheckedIMU.h` | Checked 14-byte MPU6050 read at 0x68 |
| `SolarRiderBLE.h` | BLE telemetry queue and notifications |
| `SolarRiderOTA.h` | BLE update, and the check after the new image boots |
| `EspBleOTAProtocol.h` | OTA packet order, sector CRC and commit state machine |
| `OTAPairing.example.h` | Copy this, then set your own PIN |
