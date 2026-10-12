# Parts, power and wiring

[中文](../zh-CN/hardware.md) · [Home](../../README.md)


| Item | Qty | Current basis |
| --- | --- | --- |
| ESP32-C3 classic development board | 1 | Soldered-header CORE-ESP32 style; AirM2M_CORE_ESP32C3 target, 4 MB flash |
| GY-521 MPU6050 module | 1 | Downward-soldered headers in the purchase record; I²C address 0x68, AD0 low |
| KS-59E 433 MHz receiver + matching remote | 1 receiver and 1 remote for a build | Standalone receiver purchase confirmed; left/right/stop logic outputs required |
| WS2812B rear strip, 5 V | 10 pixels in build | Cut from purchased black coated 144-pixel/m strip; ordered across the rear |
| WS2812D-F5 status pixel, 5 V | 1 | 5 mm through-hole addressable RGB; separately driven |
| Carrier PCB | 1 | EasyEDA Pro source included; check the intended board inside the project |
| Flexible solar panels, nominal 1 W / 6 V | 2 in existing design | Two 1 W / 6 V panels parallel into CN3163; measured operating voltage not supplied |
| 103040 lithium-polymer cell | 1 | Purchased reference: 3.7 V, 1200 mAh, 4.44 Wh nominal; 4.2 V charge limit; confirm protection and current |
| CN3163 solar charger | 1 | Confirmed module and terminal relationships; see power wiring guide |
| 4056-TP Type-C charging-management board | 1 | Completed board with battery/output sockets and switch; listing 1 A / 3.7 V / 4.2 V and protection |
| XR2981-style DC-DC boost board | 1 | Purchased fixed 5 V output option; verify actual voltage/load capacity |
| Power switch | As fitted | Verify switch location and current rating in the actual wiring |
| Enclosure, bag, wiring, connectors and rear-light bracket | As needed | Match the actual modules, enclosure and measured seatpost diameter |

The [parts reference](parts-reference.md) explains what is established by purchase records. Components and passives appearing only in the alternative PCB document are not an installed-build BOM.

## Signal wiring

The following mapping is authoritative for V6.5. It is a logical connection table, not a connector pin-number diagram.

| Function | GPIO | Connection |
| --- | --- | --- |
| Rear strip | 8 | WS2812 data input |
| Status pixel | 2 | Addressable pixel data input |
| Left input | 3 | Receiver left output |
| Right input | 6 | Receiver right output |
| Stop input | 10 | Receiver stop output |
| MPU6050 SDA | 4 | I²C SDA |
| MPU6050 SCL | 5 | I²C SCL |

All signal modules must share ground. The remote inputs are active-high plain `INPUT` pins; verify defined idle levels on the chosen receiver. Do not let them float. Check the receiver's output voltage before connecting it to ESP32-C3 GPIO. A 5 V supply pin is not a 5 V-tolerant GPIO. Use appropriate level translation if required. Refer to the [ESP32-C3 datasheet](https://documentation.espressif.com/ESP32-C3_Datasheet_en.pdf).

Power the addressable LEDs from a suitable regulated supply and verify the logic-level requirements of the exact strip. The LED strip arrow marks data direction; wire GPIO8 toward `DIN`. The firmware's left/right mapping assumes the existing strip orientation; check it from the rider's perspective after assembly.

## Power connections

The [power wiring guide](power-wiring.md) records the confirmed build: two panels in parallel into CN3163 SOLAR; CN3163 BAT and 4056-TP BAT share the cell; 4056-TP OUT feeds XR2981 IN; XR2981 OUT supplies the ESP32 carrier at 5 V. USB-C charges the lithium cell when solar charging is insufficient. Physical pin order, charge current and simultaneous-input behavior still need actual checks.

Use a compatible 1S lithium cell with a verified protection arrangement, insulated connections and strain relief. The supplied cell reference is 1200 mAh, not the 2000 mAh-or-larger option in the early plan. A supplier claim of protection on the management board must be checked against its actual wiring. Check polarity with a meter; matching connector shells do not guarantee matching polarity. Keep the battery away from screw tips and sharp PCB edges. A damaged or swollen battery should not be used.

## PCB source

[SolarRider_Board_20261012.epro2](../../hardware/pcb/SolarRider_Board_20261012.epro2) is a native EasyEDA Pro project. It contains two PCB documents and an older schematic page; the filename date is an export date, not proof that all documents are the same revision. Check the target PCB against the pin mapping and fitted modules before ordering. A [PCB1 fabrication archive](../../hardware/pcb/fabrication/PCB1_20260626_Gerbers.zip) is included. Its outline, top silkscreen and drill files match the corresponding manufacturing input package. It is an existing design export, not a newly DRC-verified fabrication release; inspect it together with the current module wiring.


## Existing assembled build

![Assembled carrier board](../assets/controller-assembled.jpg)

Project-supplied photograph showing the module-based assembly. It is useful for locating modules but is not a connector-polarity diagram. The photograph resembles the older carrier layout with 16-position headers in the included project; the second PCB document is not confirmed as the fabricated board.

The pictured assembly is historical and illustrates the carrier layout. If a module was replaced later, the maintainer-confirmed current module description takes precedence over the photograph.
