# Third-party notices

[English](#english) · [中文](#中文)

## English

This project installs dependencies from their upstream sources; their source code is not vendored here. Project-authored firmware is MIT-licensed, but compiled firmware also contains third-party code and must satisfy its licenses before redistribution. Library versions below match the local V6.5 build environment.

| Dependency | Version | License | Upstream |
| --- | --- | --- | --- |
| Arduino-ESP32 | 3.3.12 | LGPL-2.1; bundled components have their own notices | [Espressif](https://github.com/espressif/arduino-esp32) |
| Adafruit NeoPixel | 1.15.5 | LGPL-3.0; see upstream COPYING | [Adafruit](https://github.com/adafruit/Adafruit_NeoPixel) |
| Adafruit MPU6050 | 2.2.9 | BSD-3-Clause | [Adafruit](https://github.com/adafruit/Adafruit_MPU6050) |
| Adafruit BusIO | 1.17.4 | MIT | [Adafruit](https://github.com/adafruit/Adafruit_BusIO) |
| Adafruit Unified Sensor | 1.1.15 | Apache-2.0 | [Adafruit](https://github.com/adafruit/Adafruit_Sensor) |

Arduino Library Manager may also install Adafruit GFX / SSD1306 dependencies declared by MPU6050. Retain their upstream notices if their code is redistributed. CoreBluetooth, Foundation, AppKit and CryptoKit are Apple system frameworks used by the macOS tools; they are not redistributed in this repository.

The BLE OTA packet implementation follows Espressif's public service profile and is project source, not a copied bundled SDK. Reference: [Espressif BLE OTA](https://github.com/espressif/esp-iot-solution/tree/master/components/bluetooth/ble_ota).

EasyEDA projects may contain standard component symbols, footprints and previews. The project's hardware license covers the original design and arrangement; embedded vendor/library material keeps its own terms. Physical modules are bought separately and their manufacturers' designs are not licensed by this repository.

## 中文

**第三方说明**

本项目从上游安装依赖，不在仓库内复制库源代码。项目原创固件代码采用 MIT；编译出的固件还包含第三方代码，分发时须满足相应许可。下列版本对应 V6.5 本地编译环境。

| 依赖 | 版本 | 许可 | 上游 |
| --- | --- | --- | --- |
| Arduino-ESP32 | 3.3.12 | LGPL-2.1；所含组件另有许可 | [Espressif](https://github.com/espressif/arduino-esp32) |
| Adafruit NeoPixel | 1.15.5 | LGPL-3.0；见上游 COPYING | [Adafruit](https://github.com/adafruit/Adafruit_NeoPixel) |
| Adafruit MPU6050 | 2.2.9 | BSD-3-Clause | [Adafruit](https://github.com/adafruit/Adafruit_MPU6050) |
| Adafruit BusIO | 1.17.4 | MIT | [Adafruit](https://github.com/adafruit/Adafruit_BusIO) |
| Adafruit Unified Sensor | 1.1.15 | Apache-2.0 | [Adafruit](https://github.com/adafruit/Adafruit_Sensor) |

Arduino 库管理器可能额外安装 MPU6050 声明的 Adafruit GFX / SSD1306 依赖；分发其代码时应保留上游声明。Mac 工具使用 Apple 的系统框架，本仓库不分发这些框架。

蓝牙 OTA 实现按 Espressif 公开服务协议编写，仓库内为项目代码，不是打包复制的 SDK。参考：[Espressif BLE OTA](https://github.com/espressif/esp-iot-solution/tree/master/components/bluetooth/ble_ota)。

EasyEDA 工程可能包含标准器件符号、封装和预览。项目硬件许可覆盖原创设计与布局，内嵌的厂商或库资源保留原条款。实物模块需自行采购，其厂家设计不由本仓库授权。
