# Troubleshooting

[中文](../zh-CN/troubleshooting.md) · [Home](../../README.md)


| Symptom | Checks |
| --- | --- |
| Compile cannot find `OTAPairing.h` | Copy `OTAPairing.example.h` to `OTAPairing.h`, set the PIN, and delete the `#error` line |
| MPU startup error / red flashing | Check supply, common ground, GPIO4/5 wiring, address 0x68 and module connection |
| Yellow status persists | Keep the bicycle upright and still; check vibration, sensor readings and calibration reason |
| Purple status | Check I²C, read errors, and the supply. Purple means a bad sensor read |
| Left/right reversed | Look from the saddle and check the strip order. V6.5 is left GPIO3, right GPIO6 |
| Remote input stays active | With no button pressed the receiver should sit low. Stop is GPIO10 |
| Wrong/missing LED colors | Check strip `DIN`, GRB ordering, data level, common ground and supply under load |
| Braking indicator stays active | Stop and wait. Check the mounting vector, bag motion, and `forward_filtered` |
| BLE capture sees no rows | Enable macOS Bluetooth permission, close other connected clients, power only the intended device and wait for complete rows |
| Update is refused | Send the `.ino.bin` for this ESP32-C3 sketch. The bike stays parked, calibration finished, PIN from `OTAPairing.h` |
| Update success not confirmed | Keep the USB port free. A finished transfer still needs the board to boot the new image |
| Resets when LEDs brighten | Measure the supply while the LEDs are bright. Check the boost board, the cell, and the wires |
