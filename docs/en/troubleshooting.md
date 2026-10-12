# Troubleshooting

[中文](../zh-CN/troubleshooting.md) · [Home](../../README.md)


| Symptom | Checks |
| --- | --- |
| Compile cannot find `OTAPairing.h` | Run the local pairing helper or configure the example header |
| MPU startup error / red flashing | Check supply, common ground, GPIO4/5 wiring, address 0x68 and module connection |
| Yellow status persists | Keep the bicycle upright and still; check vibration, sensor readings and calibration reason |
| Purple status | Check I²C connection, read errors and supply stability; a purple state is not successful calibration |
| Left/right reversed | Verify rider-view orientation, strip order and receiver mapping; V6.5 uses left GPIO3 / right GPIO6 |
| Remote input stays active | Check receiver mode, idle levels and active-high output mapping; stop input is GPIO10 |
| Wrong/missing LED colors | Check strip `DIN`, GRB ordering, data level, common ground and supply under load |
| Braking indicator stays active | Stop and let a stable reference recover; inspect mounting vector, bag motion and `forward_filtered`; do not assume road behavior is fixed |
| BLE capture sees no rows | Enable macOS Bluetooth permission, close other connected clients, power only the intended device and wait for complete rows |
| Update is refused | Use a C3 application image with the OTA marker; confirm parked/still state, free OTA slot, pairing and compatible profile |
| Update success not confirmed | Keep USB recovery available; inspect identity/boot state and do not infer success from a completed transfer alone |
| Resets when LEDs brighten | Measure supply sag and connections; confirm regulator capacity, battery condition and wiring |

When reporting a problem, include firmware/core versions, actual modules, mounting, reproduction steps and sanitized observations. Do not attach private pairing headers or raw device-identifying logs.
