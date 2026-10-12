# Bluetooth

[中文](../zh-CN/bluetooth.md) · [Home](../../README.md)

Advertised name: `SolarRider`.

Notify service `6E400001-B5A3-F393-E0A9-E50E24DCCA9E`, characteristic `6E400003-B5A3-F393-E0A9-E50E24DCCA9E`. CSV rows in 20-byte notifications. Reassemble at the newline. BLE up to 10 rows/s, USB up to 50.

`t_us,seq,loop_dt_us,ax,ay,az,gx,gy,gz,gravity_x,gravity_y,gravity_z,forward_raw,forward_filtered,old_diff_y,old_hits,old_brake,ready,brake,events,mode,dropped,cal_reason,acc_std_max,gyro_std_max,gravity_error`

Acceleration is m/s², gyro is rad/s. `cal_reason`: 0 ready, 1 collecting, 2 acceleration, 3 gyro, 4 bad read, 5 gap, 6 bad magnitude. `old_*` is the older detector. The lights follow `brake`. `dropped` counts USB debug lines that did not fit.

OTA service `0x8018`: firmware `0x8020`, progress `0x8021`, command `0x8022`, information `0x8023`. 4 KiB sectors, CRC16, in order. The image has to be an ESP32-C3 application with the OTA marker. Protocol id `SolarRider_OTA_PROTOCOL_1`. Send the `.ino.bin`. Bike parked. PIN is the one in `OTAPairing.h`.
