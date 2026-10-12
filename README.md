# SolarRide

[中文](README.zh-CN.md) · [Build guide](docs/en/assembly.md) · [Firmware](docs/en/firmware.md) · [Issues](https://github.com/Andy-Hyy/SolarRide/issues)

A DIY bicycle bag lighting project with wireless turn signals, an IMU-triggered braking light, and a solar-assisted battery power system. An ESP32-C3 drives a 10-pixel rear strip and a status pixel; a handlebar remote selects left, right or normal riding mode.

The files here are meant for making your own version, understanding how it works, and improving the design. Firmware, PCB sources, enclosure models and small macOS Bluetooth tools are included.

![Bicycle-mounted project](docs/assets/bicycle-mounted.jpg)

![System overview](docs/assets/system-overview.svg)

## What it does

- Red animated rear light in normal mode; amber flowing patterns for left and right turns.
- An acceleration-based braking indication that temporarily takes priority over the turn pattern.
- A status pixel for calibration, normal operation, turning and sensor problems.
- Live USB and BLE telemetry; a BLE firmware update implementation for parked maintenance.
- A module-based PCB and two enclosure approaches: a screw-fastened version and a sliding-frame version.

The current source is V6.5. On the existing build, these checks are done:

- USB programming
- Left, right, and stop from the handlebar remote
- Basic lighting, including a push-and-stop brake flash
- The brake warning after the box was mounted on the bike and the threshold was recalibrated for that tilt

These are not done:

- Battery runtime
- How much the solar panels actually charge the cell
- Trigger accuracy, repeated false flashes, or the same check on different roads
- A complete Bluetooth firmware update and recovery on the device

One mounted brake check is not a measurement of road durability.

Purchase references identify the CN3163 charger, a 3.7 V / 1200 mAh battery, fixed 5 V boost, 1 W / 6 V panels and lighting parts. The complete terminal relationships are recorded in the [power guide](docs/en/power-wiring.md): CN3163 solar charging and 4056-TP USB charging share the cell; 4056-TP output feeds the XR2981 5 V boost.

This is an auxiliary DIY light. It does not operate the bicycle's brakes and should not replace normal bicycle lights or hand signals. The current brake indication flashes at approximately 10 Hz; review the pattern and mounting for your intended use.

## Start here

1. Read the [parts and wiring guide](docs/en/hardware.md) and [assembly guide](docs/en/assembly.md).
2. Set a private local Bluetooth PIN and [build/upload the firmware](docs/en/firmware.md).
3. [Calibrate the mounting direction](docs/en/calibration.md), then check the controls and light directions while stationary.
4. Follow the [operation guide](docs/en/operation.md). Use [Bluetooth tools](docs/en/bluetooth.md) when useful.

## Contents

| Folder | Contents |
| --- | --- |
| `firmware/` | V6.5 Arduino sketch and reproducible build profile |
| `hardware/pcb/` | Native EasyEDA Pro project and existing PCB1 Gerber/drill archive |
| `hardware/enclosure/` | Original STEP models |
| `tools/` | Local pairing setup, build and regression scripts |
| `tools/macos/` | BLE capture and update source tools |
| `tests/` | Host regression tests and sanitized build-specific data |
| `docs/en/`, `docs/zh-CN/` | English and Chinese guides |

No private device PINs or prebuilt firmware images are distributed. Build a device-specific image locally.

More reading: [parts reference](docs/en/parts-reference.md), [design](docs/en/design.md), [enclosures](docs/en/enclosure.md), [checks and limitations](docs/en/testing.md), [troubleshooting](docs/en/troubleshooting.md), [roadmap](docs/en/roadmap.md), [licensing](docs/en/licensing.md).

## Contribute

Build reports, clearer instructions, component substitutions and focused improvements are welcome. See [CONTRIBUTING](CONTRIBUTING.md), [community guidelines](CODE_OF_CONDUCT.md) and [security reporting](SECURITY.md). Maintained by [Andy-Hyy](https://github.com/Andy-Hyy).

## Licenses

| Material | License |
| --- | --- |
| Original software | [MIT](LICENSES/MIT.txt) |
| Original circuit and enclosure designs | [CERN-OHL-P-2.0](LICENSES/CERN-OHL-P-2.0.txt) |
| Documentation, original diagrams and data | [CC BY 4.0](LICENSES/CC-BY-4.0.txt) |

See [LICENSE](LICENSE) for file scope and [third-party notices](THIRD_PARTY_NOTICES.md) for dependency terms.
