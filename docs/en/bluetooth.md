# Bluetooth data and maintenance

[中文](../zh-CN/bluetooth.md) · [Home](../../README.md)


## Read-only telemetry

The device advertises as `SolarRider`. Service `6E400001-B5A3-F393-E0A9-E50E24DCCA9E` provides notifications on `6E400003-B5A3-F393-E0A9-E50E24DCCA9E`. Rows are ASCII CSV split into 20-byte notifications; clients must reassemble complete newline-terminated rows. BLE sends up to 10 rows/s and may drop intermediate rows. USB targets up to 50 rows/s. Neither transport is a persistent onboard log.

I read this on my Mac with a small program that is not in this repository. It picks the first device named `SolarRider`, so I only power the box I mean to read. Close other connected clients. Rows can arrive in pieces.

The V6.5 row has 26 columns: `t_us,seq,loop_dt_us,ax,ay,az,gx,gy,gz,gravity_x,gravity_y,gravity_z,forward_raw,forward_filtered,old_diff_y,old_hits,old_brake,ready,brake,events,mode,dropped,cal_reason,acc_std_max,gyro_std_max,gravity_error`. Acceleration is m/s²; gyro is rad/s. Calibration reasons are 0 ready, 1 collecting, 2 acceleration motion, 3 gyro motion, 4 invalid/read failure, 5 gap, 6 invalid magnitude. `dropped` counts USB debug writes that could not fit, not a guaranteed BLE loss counter.

## BLE firmware updates

The phone-side update program is also only on my computer. I have not put it here. If you try an update, keep the bike parked, wait until the box finishes calibration, and use the PIN from your own `OTAPairing.h`. I have not completed a full wireless update, restart, and recovery on the real device. Keep the USB port reachable.

OTA service `0x8018` uses firmware `0x8020`, progress `0x8021`, command `0x8022`, and information `0x8023`. The transfer uses 4 KiB sectors, CRC16 and ordered packets. The receiver checks the C3 application identity, size and OTA marker. CRC and image hashes detect broken data. They are not a signature. Later versions should keep `SolarRider_OTA_PROTOCOL_1` and the receiver in the sketch. A rollback hook depends on the bootloader. It is not a promise that a bad update will recover.
