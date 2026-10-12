# Lights

[中文](../zh-CN/operation.md) · [Home](../../README.md)

| State | Status pixel |
| --- | --- |
| Calibrating | Dim yellow |
| Ready | Dim cyan |
| Turn | Amber blink |
| Brake | Red |
| Sensor read failed | Purple |
| MPU initialization failed | Red flashing |
| BLE update | Blue pulse |

The rear strip has 10 pixels. In the normal pattern, two red pixels move outward from the center. Left and right select amber chase patterns in opposite directions. Stop on the remote restores the normal red pattern. A brake warning overrides the current pattern and restores the previous mode when it ends. The brake pattern toggles every 50 ms.
