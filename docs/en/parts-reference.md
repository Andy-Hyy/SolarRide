# Parts I used

[中文](../zh-CN/parts-reference.md) · [Home](../../README.md) · [Hardware](hardware.md) · [Power wiring](power-wiring.md)

| Part | What I bought |
| --- | --- |
| ESP32-C3 board | CORE-ESP32 style, headers soldered, 4 MB flash. Arduino target: AirM2M CORE ESP32C3 |
| GY-521 MPU6050 | Headers soldered downward. The sketch uses I²C address 0x68 |
| KS-59E receiver and remote | 433 MHz. The receiver outputs left, right, and stop as high signals. Listing says the receiver runs on 5 V |
| CN3163 solar charger | Blue connector board. Listing says DC 4.4–6 V in. Two panels go in parallel to SOLAR. BAT goes to the cell |
| 4056-TP Type-C charger | Finished board with battery and output sockets, a switch, and status LEDs. Listing says 1 A, 3.7 V / 4.2 V. Type-C charges the cell. OUT goes to the XR2981 |
| XR2981 boost | The fixed 5 V version, with a “2R2” inductor. IN comes from the 4056-TP output. OUT feeds the ESP32 board |
| 103040 cell | Listing: 3.7 V, 1200 mAh, 4.44 Wh, 4.2 V charge limit, about 10 × 30 × 40 mm |
| Flexible solar panels | Two panels, listing 1 W / 6 V each, wired in parallel |
| WS2812B strip | 5 V, black PCB, coated, 1 m, 144 pixels/m. I cut 10 pixels |
| WS2812D-F5 status pixel | One 5 mm through-hole pixel. The drawing shows 1 DOUT, 2 VDD, 3 GND, 4 DIN. I use DIN, VDD, and GND |
| Frame bag | The listing had 1.5 L and 2.7 L sizes |
| Seatpost bracket | A clamp in the F321 family. I measured the seatpost on the bike |

The other PCB page in the EasyEDA file has parts I did not put on the board I built.
