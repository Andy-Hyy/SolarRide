# Lights

[中文](../zh-CN/operation.md) · [Home](../../README.md)

| State | Status pixel |
| --- | --- |
| Calibrating | Dim yellow |
| Ready | Dim cyan |
| Turn | Amber blink |
| Brake | Red |
| Bad sensor read | Purple |
| MPU init failed | Red flashing |
| BLE update | Blue, pulsing |

Ten rear pixels. Normal mode is two red pixels moving out from the center. Left and right are amber in opposite directions. Stop on the remote returns to the normal red pattern. Brake covers that pattern, then the previous mode comes back. The brake pattern toggles every 50 ms.
