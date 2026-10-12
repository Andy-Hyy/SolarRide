# Build and assemble

[中文](../zh-CN/assembly.md) · [Home](../../README.md)


## Before starting

Read the hardware guide and the confirmed [power wiring](power-wiring.md) before assembly. Have a multimeter, soldering tools, insulated wire, a USB data cable and a way to current-limit the initial supply. Work on the electronics with the battery disconnected.

## Assembly order

1. Import the EasyEDA Pro project. Identify the intended PCB, check module footprint spacing and compare all connector nets to the firmware mapping. Export fabrication files only after this review.
2. Solder low-profile passives and connectors, then install the modules. Check electrolytic capacitor orientation and avoid solder bridges. Verify ground and supply continuity with power disconnected.
3. Test the regulator output without the ESP32 and LEDs connected. Connect the control electronics only once the voltage and polarity are correct. Follow the confirmed power chain for battery/solar connections.
4. Connect SDA/SCL and the active-high remote outputs. Connect the main strip's `DIN`, supply and ground, plus the separate status pixel. Confirm 10 rear pixels and the actual view direction.
5. Build and upload the firmware by USB with the rear strip visible. Check startup, status pixel and each remote function before putting the board into the enclosure.
6. Inspect and slice the chosen STEP models. The screw-fastened design is the version physically printed; the A sliding-frame version has not been printed. Printing material, settings, screw sizes and clearances need to be selected and checked for your printer and build. Start with a fit check before a full enclosure print.
7. Secure the board and IMU. Keep USB access, insulate solder joints, and route cables without pinching. Secure the battery without compression. Keep the remote and LED wiring clear of wheels, chain and steering.
8. Mount the bag rigidly enough to preserve sensor orientation. Run the direction calibration and stationary control checks, followed by controlled push/stop checks in a clear area.

Do not assume a printed enclosure or coated LED strip makes the completed device waterproof. Seams, cable exits and module exposure need their own inspection and testing. Do not charge inside a closed enclosure without checking temperature and ventilation.

Record the actual modules, wiring, enclosure files and changes you used. These details help other makers reproduce your build.
