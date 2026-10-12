# Wiring

[中文](../zh-CN/hardware.md) · [Home](../../README.md) · [Power](power-wiring.md)

| Function | GPIO |
| --- | --- |
| Rear strip `DIN` | 8 |
| Status pixel `DIN` | 2 |
| Remote left | 3 |
| Remote right | 6 |
| Remote stop | 10 |
| MPU6050 SDA | 4 |
| MPU6050 SCL | 5 |

Common ground. Remote inputs are active-high. GPIO is not a 5 V input. The strip and the status pixel use the 5 V rail.

| Part | Note |
| --- | --- |
| ESP32-C3 CORE board | AirM2M_CORE_ESP32C3, 4 MB |
| GY-521 MPU6050 | 0x68, AD0 low |
| KS-59E receiver and remote | 433 MHz |
| WS2812B strip | 10 pixels, cut from 144/m coated strip |
| WS2812D-F5 | 5 mm. Use DIN, VDD, GND |
| CN3163 | panel input |
| 4056-TP | USB-C charger, output to the XR2981 |
| XR2981 | fixed 5 V, inductor marked 2R2 |
| 103040 cell | 3.7 V, 1200 mAh, charge to 4.2 V |
| Solar panels | two, 1 W / 6 V, parallel |
| Frame bag | listing 1.5 L and 2.7 L |
| Seatpost clamp | F321, sized on this bike |

![Assembled board](../assets/controller-assembled.jpg)

The photo is the PCB page with 16 pins on each side. EasyEDA file: [SolarRider_Board_20261012.epro2](../../hardware/pcb/SolarRider_Board_20261012.epro2). Gerbers: [PCB1_20260626_Gerbers.zip](../../hardware/pcb/fabrication/PCB1_20260626_Gerbers.zip).
