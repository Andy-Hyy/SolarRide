# Build and upload firmware

[中文](../zh-CN/firmware.md) · [Home](../../README.md)


## Source and toolchain

Open `firmware/SolarRider_BLE_OTA_V6_5_20261011/SolarRider_BLE_OTA_V6_5_20261011.ino`. Keep its folder name intact so Arduino recognizes the sketch.

The checked profile is Arduino-ESP32 **3.3.12**, board **AirM2M CORE ESP32C3**, FQBN `esp32:esp32:AirM2M_CORE_ESP32C3`, 4 MB flash and default OTA-capable partitions. Use [Espressif's installation instructions](https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html). The checked library versions are in [build-profile.json](../../firmware/build-profile.json). BLE/Wire/Update come with the core. Library Manager may install additional declared Adafruit dependencies.

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

I upload from the Arduino IDE. Copy the pairing header before you compile. These commands only prepare the compiler. They do not flash the board.

## First USB upload

In Arduino IDE, select the checked board/profile, choose the actual connected port and upload. Open the serial monitor at **115200 baud**. Keep USB recovery accessible while trying your own firmware. A different board, flash layout or library version needs its own checks; do not assume all ESP32-C3 boards have the same pin layout.

The default partition table has two application slots of `0x140000` bytes each plus OTA metadata. Do not choose a No OTA partition scheme if wireless updates are needed. For later BLE updates, select only the generated `.ino.bin` **application** image, never the merged, bootloader or partitions image.

## Source layout

| File | Role |
| --- | --- |
| `.ino` | GPIO, remote modes, lamps, telemetry and main loop |
| `BrakeDetector.h` | Gravity/bias estimation and deceleration indication |
| `CheckedIMU.h` | Checked 14-byte MPU6050 read at 0x68 |
| `SolarRiderBLE.h` | BLE telemetry queue and notifications |
| `SolarRiderOTA.h` | BLE OTA integration, image checks and boot-health hooks |
| `EspBleOTAProtocol.h` | OTA packet order, sector CRC and commit state machine |
| `OTAPairing.example.h` | Public template for private local configuration |

The historical firmware identifier still contains `TRIAL_V6_5`; it is retained for tool compatibility, not a new maturity claim. The publicly compiled source has the same operational logic as the existing V6.5, with a separately configured PIN. A newly compiled image has its own hash and is not the previously uploaded image.
