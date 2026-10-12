# 供电

[English](../en/power-wiring.md) · [首页](../../README.md) · [接线](hardware.md)

![供电连接](../assets/power-wiring.svg)

| 从 | 到 |
| --- | --- |
| 两块光伏板，同极 | CN3163 SOLAR，并联 |
| CN3163 BAT | 电池 |
| 4056-TP 电池端 | 同一节电池 |
| USB-C | 4056-TP 的 Type-C |
| 4056-TP OUT | XR2981 IN |
| XR2981 OUT | ESP32 的 5 V |

XR2981 的输入是 4056-TP 的输出，不是电池。接 ESP32 之前量过 5 V。电池负极没有和 4056-TP 输出负极短在一起。
