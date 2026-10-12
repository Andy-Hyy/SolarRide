# Faults

[中文](../zh-CN/troubleshooting.md) · [Home](../../README.md)

| What you see | Look at |
| --- | --- |
| No `OTAPairing.h` | Copy the example, set the PIN, delete `#error` |
| Red flashing | 5 V, ground, GPIO4/5, address 0x68 |
| Stays yellow | Bike still and upright |
| Purple | I²C and supply. The read failed |
| Left and right swapped | View from the saddle. Left is GPIO3, right is GPIO6 |
| A remote input stuck on | Receiver should idle low. Stop is GPIO10 |
| Wrong colors | `DIN`, GRB, ground, 5 V under load |
| Brake light stuck on | Mounting vector, bag motion, `forward_filtered` |
| No BLE rows | One client, this box only, wait for a full line |
| Update refused | `.ino.bin`, bike parked, calibration done, PIN in `OTAPairing.h` |
| Update sent, board unchanged | USB, and whether it booted the new image |
| Reset when the LEDs get bright | 5 V while the strip is on |
