# Parts reference

[中文](../zh-CN/parts-reference.md) · [Home](../../README.md) · [Hardware](hardware.md) · [Power wiring](power-wiring.md)

The reference build uses the following module families. Specifications come from the supplied purchase records, seller listings, source files and maintainer-confirmed wiring. Nominal ratings are not measured runtime or performance. Equivalent parts must match the electrical and mechanical interfaces.

| Part | Reference specification | Selection details |
| --- | --- | --- |
| ESP32-C3 classic development board | CORE-ESP32 style, soldered headers, USB data cable; AirM2M CORE ESP32C3 build target, 4 MB flash | Match header spacing, exposed GPIOs and USB interface; not an arbitrary C3 mini board |
| GY-521 MPU6050 | Downward-soldered headers; six-axis IMU | Firmware expects I²C address 0x68; match supply/I²C levels and module footprint |
| KS-59E receiver and matching remote | 433 MHz, decoded active-high outputs, seller-listed 5 V receiver supply | Left, right and stop must drive the expected GPIOs. Choose suitable receiver mode and verify output levels. Match the actual transmitter, not the listing's mixed 315/433 wording |
| CN3163 solar charger | Blue connector-board style; supplier advertises DC 4.4–6 V input | Two panels parallel into SOLAR; BAT shares the cell terminals. Check exact-module input limits, current setting and polarity |
| 4056-TP Type-C lithium charging-management board | Completed board with battery/output sockets, switch and status LEDs; listing states 1 A, 3.7 V / 4.2 V and protection | Type-C charges the cell; BAT connects to the cell; OUT supplies the XR2981 input. Charging current and protection behavior still require actual checks |
| XR2981 boost board | Purchased fixed 5 V output option; pictured with “2R2” inductor | Not the 9 V/12 V options. IN receives 4056-TP output, OUT supplies the ESP32 control board |
| 103040 lithium-polymer cell | 3.7 V nominal, 1200 mAh, 4.44 Wh stated, 4.2 V charge limit; approximately 10 × 30 × 40 mm | Supplier ratings, not measured usable capacity. Verify cell/protection compatibility and current limits |
| Flexible solar panels | 1 W / 6 V nominal, two panels in the existing build | Connect in parallel; placement, shading and actual open-circuit voltage matter. Panel material wording is a supplier description, not independently verified |
| WS2812B rear strip | Purchased roll: 5 V, black PCB, coated, 1 m, 144 pixels/m | Cut 10 pixels for the current firmware. Seller's IP65 wording concerns the strip, not the completed enclosure or cut ends |
| WS2812D-F5 status pixel | 5 V addressable RGB, 5 mm through-hole, four leads | Use one. Depicted pin functions: 1 DOUT, 2 VDD, 3 GND, 4 DIN; check actual package orientation |
| Frame bag | BUE listing offers 1.5 L and 2.7 L | Exact selected capacity not established; fit the bicycle, panels and enclosure |
| Rear-light seatpost bracket | Clamp/slot family, including F321 and multiple diameters | Measure seatpost and light mounting points; the supplied selection does not establish one diameter |

The firmware needs 10 rear pixels plus one status pixel, one sensor, one receiver and a compatible transmitter. Purchased roll length, spare-module orders and supplier quantity selectors are not installed-build quantities. Components and passives appearing only in the alternative PCB document are not a BOM for the photographed PCB1 carrier.

For the depicted WS2812D-F5, only DIN, VDD and GND are used for the single pixel; DOUT is unused. Lead position must be checked against the actual package.

Original order screenshots are retained privately. Public files omit order identifiers, delivery details, seller images and historical prices. Repeated screenshots of the same order are not counted twice. The seller's “imported original” IMU SKU wording does not establish authenticity. No current availability or supplier endorsement is implied.
