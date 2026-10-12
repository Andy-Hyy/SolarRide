# Power wiring

[中文](../zh-CN/power-wiring.md) · [Home](../../README.md) · [Hardware](hardware.md)

This is how I wired the box.

![Power connections](../assets/power-wiring.svg)

## Terminal connections

| Source | Destination | Relationship |
| --- | --- | --- |
| Both panel positives / negatives | CN3163 SOLAR+ / SOLAR− | Two 1 W / 6 V panels in parallel, matching polarity |
| CN3163 BAT+ / BAT− | Cell positive / negative | Solar charger battery terminals connect to the cell |
| 4056-TP battery positive / negative | Same cell positive / negative | Both charging-management boards share the cell terminals |
| USB-C supply | 4056-TP Type-C input | Wired charging of the cell when solar charging is insufficient |
| 4056-TP OUT+ / OUT− | XR2981 IN+ / IN− | Load supply comes from the management board's output |
| XR2981 OUT+ / OUT− | ESP32 carrier 5 V / supply return | Fixed 5 V. I measured it before connecting |

The carrier sends that 5 V to the strip and the status LED. The XR2981 input is the 4056-TP output, not the cell. The 5 V comes from the XR2981.

I did not tie the cell negative straight across to the protected output negative. The signal modules share the return on the controller side. I checked polarity with a meter. The color of a plug did not tell me which contact was positive.

USB-C charges the cell when the panels are not doing much. I have not measured charge current with both inputs connected at once.

I measured the XR2981 output before I plugged in the ESP32. The old schematic in the EasyEDA file does not show this whole chain. The chain is this table plus the wires on the modules.
