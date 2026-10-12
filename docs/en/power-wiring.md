# Power

[中文](../zh-CN/power-wiring.md) · [Home](../../README.md) · [Wiring](hardware.md)

![Power connections](../assets/power-wiring.svg)

| From | To |
| --- | --- |
| Both panels, same polarity | CN3163 SOLAR, parallel |
| CN3163 BAT | Cell |
| 4056-TP BAT | Same cell |
| USB-C | 4056-TP Type-C input |
| 4056-TP OUT | XR2981 IN |
| XR2981 OUT | ESP32 5 V |

The XR2981 input is the 4056-TP output, not the cell. 5 V was measured before the ESP32 was connected. Cell negative is not tied to the 4056-TP output negative.
