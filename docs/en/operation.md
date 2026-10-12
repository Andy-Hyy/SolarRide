# Use the lights

[中文](../zh-CN/operation.md) · [Home](../../README.md)


1. I check the bag, the cell, and the wires. Then I turn it on and hold the bike still until the status pixel goes cyan.
2. Before I ride, I try left and right from the saddle. Stop on the remote only puts the lights back on the normal red pattern.
3. While the brake warning is on, it covers the turn pattern. Afterward the lights go back to the mode I had selected. The remote only changes the lights.
4. If the lights look wrong, I stop, look at the status pixel, and turn the power off before I touch a wire.
5. I try a firmware update with the bike parked and the USB port free.

## Status pixel

| State | Status pixel | Meaning |
| --- | --- | --- |
| Startup/calibrating | Dim yellow | Waiting for stationary reference |
| Normal and ready | Dim cyan | Detector ready, normal mode |
| Turning | Amber blink | Left or right mode selected |
| Brake state | Red | Current brake indication active |
| Bad sensor read | Purple | The reading failed |
| MPU initialization failed | Red flashing | Firmware cannot initialize sensor |
| OTA maintenance | Blue pulsing | Ride lamp suspended during update |

The rear brake light toggles every 50 ms. To change that I edit the sketch and upload again. I don't do that while riding.
