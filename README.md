# SolarRide

I built this for my bike. It sits on the bag. A remote on the handlebar switches the turn lights. An accelerometer turns on a brake warning when I slow down. A small solar panel and a USB-C board charge the battery. The controller is an ESP32-C3.

This is a high-school prototype.

## What is in this repository

- `firmware/` is the Arduino sketch I am using, V6.5. The folder name has to stay the same or Arduino will not open it. The Bluetooth name is still `SolarRider`.
- `hardware/pcb/` is the EasyEDA project and the Gerber files for the green board.
- `hardware/enclosure/` is the STEP files for the box.
- `docs/` is my notes on wiring, the box, and what I have actually checked. English and Chinese are both there.

The first board was purple. The ground and the LED connector were wrong, and reworking it did not fix it. I redrew the layout and ordered a green board that fits JLCPCB's free small-board size.

## What I have checked

- USB upload
- Left, right, and stop from the remote
- The lights, including a brake flash when I push the bike and stop
- The brake warning after I mounted the box and changed the threshold for that tilt

On the bench, the brake light did not match the bike. The box sits at a different angle than it did on the table.

## What I have not measured

- How long the battery lasts
- How much the solar panels actually charge the cell
- How often the brake light is right, or whether it flashes by mistake on other roads
- A full Bluetooth firmware update and recovery on the device

## If you build one

Open `firmware/SolarRider_BLE_OTA_V6_5_20261011/SolarRider_BLE_OTA_V6_5_20261011.ino`. Copy `OTAPairing.example.h` to `OTAPairing.h`, put in your own six-digit PIN, and delete the `#error` line. Do not commit that file.

More detail is in [docs/en](docs/en/hardware.md) and [docs/zh-CN](docs/zh-CN/hardware.md).
