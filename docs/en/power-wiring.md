# Power wiring

[中文](../zh-CN/power-wiring.md) · [Home](../../README.md) · [Hardware](hardware.md)

This is the maintainer-confirmed connection structure of the existing build. It identifies terminal relationships, not a connector's physical pin order or an independent validation of charging behavior.

![Power connections](../assets/power-wiring.svg)

## Terminal connections

| Source | Destination | Relationship |
| --- | --- | --- |
| Both panel positives / negatives | CN3163 SOLAR+ / SOLAR− | Two 1 W / 6 V panels in parallel, matching polarity |
| CN3163 BAT+ / BAT− | Cell positive / negative | Solar charger battery terminals connect to the cell |
| 4056-TP battery positive / negative | Same cell positive / negative | Both charging-management boards share the cell terminals |
| USB-C supply | 4056-TP Type-C input | Wired charging of the cell when solar charging is insufficient |
| 4056-TP OUT+ / OUT− | XR2981 IN+ / IN− | Load supply comes from the management board's output |
| XR2981 OUT+ / OUT− | ESP32 carrier 5 V / supply return | Fixed 5 V regulated supply; verify measured voltage before connecting |

The carrier distributes its 5 V rail to the main strip and status LED according to the existing PCB design. The boost input is not documented as a direct connection to the cell. The 4056-TP output is not the fixed 5 V source; fixed 5 V is produced by XR2981.

Do not bridge cell-negative to protected-output-negative merely because both are called ground: check the protection topology and keep the documented terminal path intact. All downstream signal modules share the control-side supply return. Connector colors and shapes do not establish polarity.

## Charging and checks

USB-C is the alternative charging input used when solar cannot supply useful charging. Concurrent solar/USB charging, charge current, protection thresholds, reverse-current handling and temperature behavior have not been characterized. This guide does not establish safe simultaneous-input operation or runtime.

With load power disconnected, confirm terminal polarity and module identity. Measure XR2981 output before attaching the ESP32, then check supply sag and temperature with the actual light patterns. Confirm the configured charge current is appropriate for the 1200 mAh cell using the module and cell specifications. Replacing the management board with a bare charger board can change its switch, protection and output behavior.

The PCB files include external connections and mounting provisions; the complete charging path also relies on module wiring. Read this guide together with the hardware source rather than assuming all relationships are visible in the old schematic alone.
