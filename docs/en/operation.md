# Use the lights

[中文](../zh-CN/operation.md) · [Home](../../README.md)


1. Check that the bag, battery and wiring are secure. Turn on the supply and keep the bicycle upright and still until calibration completes.
2. Test left and right from the rider's perspective before riding. The stop function returns to the regular red pattern; it does not mechanically stop the bicycle or turn off power.
3. A detected braking state takes priority over the selected turn pattern. The saved mode resumes afterward. The remote is not linked to the bicycle's mechanical brakes.
4. If behavior is unexpected, stop in a safe place and inspect the status pixel and telemetry. Turn off the supply before handling wiring.
5. Perform firmware updates while parked, with stable power and USB recovery accessible.

## Status pixel

| State | Status pixel | Meaning |
| --- | --- | --- |
| Startup/calibrating | Dim yellow | Waiting for stationary reference |
| Normal and ready | Dim cyan | Detector ready, normal mode |
| Turning | Amber blink | Left or right mode selected |
| Brake state | Red | Current brake indication active |
| Runtime invalid sensor | Purple | Read/plausibility failure |
| MPU initialization failed | Red flashing | Firmware cannot initialize sensor |
| OTA maintenance | Blue pulsing | Ride lamp suspended during update |

The rear braking pattern toggles every 50 ms, approximately 10 complete flashes per second. Its timing is adjustable in the sketch; changing it requires a rebuild and a visible functional check. Never tune or operate maintenance tools while riding.

No battery percentage display, automatic persistent ride log, phone turn-control app or automatic solar-energy management is implemented by the current firmware. Charging is handled by the fitted hardware modules.
