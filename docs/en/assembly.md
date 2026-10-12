# Build and assemble

[中文](../zh-CN/assembly.md) · [Home](../../README.md)


## Before starting

Read the hardware page and the [power wiring](power-wiring.md) first. I use a meter, a soldering iron, wire, and a USB cable. The battery stays unplugged while I solder.

## How I put it together

1. I opened the EasyEDA file and used the PCB page with 16 pins on each side. The Gerbers are the June 2026 zip.
2. I soldered the small parts and the connectors first, then the modules. I checked the electrolytic capacitors and looked for solder bridges. With the battery unplugged, I checked ground and the supply.
3. I measured the XR2981 before the ESP32 and the LEDs were connected. The cell and the panels follow the power wiring page.
4. I wired SDA, SCL, and the remote outputs, then the strip `DIN`, power, ground, and the status pixel. The rear strip is 10 pixels.
5. I uploaded over USB with the strip where I could see it. I tried startup, the status pixel, and the remote before the board went into the box.
6. I printed the screw-fastened box. The sliding frame is still only the STEP file.
7. The board and the sensor are fixed in the box. The USB port is still reachable. The wires are not pinched, and the cell is not crushed. The remote cable and the strip stay clear of the wheel, the chain, and the bars.
8. Mount the bag so the sensor stays put. Calibrate the direction, then try the lights while the bike is still, then a push-and-stop in a clear spot.
