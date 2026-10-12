# 接线

[English](../en/hardware.md) · [首页](../../README.zh-CN.md) · [供电](power-wiring.md)

| 功能 | GPIO |
| --- | --- |
| 尾灯 `DIN` | 8 |
| 状态灯 `DIN` | 2 |
| 遥控左转 | 3 |
| 遥控右转 | 6 |
| 遥控停止 | 10 |
| MPU6050 SDA | 4 |
| MPU6050 SCL | 5 |

信号共地。遥控输入为高电平有效。GPIO 不能作为 5 V 输入。灯带与状态灯由 5 V 供电。

| 零件 | 备注 |
| --- | --- |
| ESP32-C3 CORE 板 | AirM2M_CORE_ESP32C3，4 MB |
| GY-521 MPU6050 | 地址 0x68，AD0 接低 |
| KS-59E 接收器和遥控器 | 433 MHz |
| WS2812B 灯带 | 10 颗，取自 144 颗/米滴胶灯带 |
| WS2812D-F5 | 5 mm。使用 DIN、VDD、GND |
| CN3163 | 光伏输入 |
| 4056-TP | USB-C 充电，输出接至 XR2981 |
| XR2981 | 固定 5 V，电感标注 2R2 |
| 103040 电池 | 3.7 V，1200 mAh，充电上限 4.2 V |
| 太阳能板 | 两块 1 W / 6 V，并联 |
| 车包 | 标称容量 1.5 L 与 2.7 L |
| 座管夹 | F321，按本车座管尺寸 |

![已组装的控制板](../assets/controller-assembled.jpg)

照片对应每侧 16 针的 PCB 页面。EasyEDA 工程：[SolarRider_Board_20261012.epro2](../../hardware/pcb/SolarRider_Board_20261012.epro2)。Gerber 文件：[PCB1_20260626_Gerbers.zip](../../hardware/pcb/fabrication/PCB1_20260626_Gerbers.zip)。
