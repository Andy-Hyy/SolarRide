# Faults

[中文](../zh-CN/troubleshooting.md) · [Home](../../README.md)

| Symptom | Check |
| --- | --- |
| `OTAPairing.h` is missing | Copy the example, enter the PIN, and remove `#error` |
| Red flashing | 5 V, common ground, GPIO4/5, address 0x68 |
| Yellow remains on | Keep the bicycle upright and stationary |
| Purple | I²C and supply. The sensor read failed |
| Left and right are reversed | Check from the rider's view. Left is GPIO3, right is GPIO6 |
| A remote input stays active | The receiver should idle low. Stop is GPIO10 |
| Incorrect colors | `DIN`, GRB order, common ground, and 5 V while the strip is lit |
| Brake warning stays on | Mounting vector, bag motion, and `forward_filtered` |
| No BLE rows | Connect only this unit and wait for a complete line |
| Update refused | Use `.ino.bin`. The bicycle is stationary, calibration is complete, and the PIN is in `OTAPairing.h` |
| Transfer finished, firmware unchanged | USB access, and whether the new image booted |
| Reset when the strip brightens | Measure 5 V while the strip is lit |
