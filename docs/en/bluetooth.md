# Bluetooth

[中文](../zh-CN/bluetooth.md) · [Home](../../README.md)

Advertised name: `SolarRider`.

Notifications use service `6E400001-B5A3-F393-E0A9-E50E24DCCA9E` and characteristic `6E400003-B5A3-F393-E0A9-E50E24DCCA9E`. Rows are CSV, split into 20-byte notifications and reassembled at the newline. BLE sends up to about 10 rows/s, and USB up to about 50 rows/s.

`t_us,seq,loop_dt_us,ax,ay,az,gx,gy,gz,gravity_x,gravity_y,gravity_z,forward_raw,forward_filtered,old_diff_y,old_hits,old_brake,ready,brake,events,mode,dropped,cal_reason,acc_std_max,gyro_std_max,gravity_error`

Acceleration is in m/s² and angular velocity is in rad/s. `cal_reason`: 0 ready, 1 collecting, 2 acceleration, 3 gyro, 4 read failure, 5 gap, 6 invalid magnitude. The `old_*` fields are the previous detector. The lamps follow `brake`. `dropped` counts USB debug lines that did not fit in the buffer.

OTA service `0x8018`: firmware `0x8020`, progress `0x8021`, command `0x8022`, information `0x8023`. Transfers use 4 KiB sectors, CRC16, and sequential packets. The image must be an ESP32-C3 application that carries the OTA marker. The protocol identifier is `SolarRider_OTA_PROTOCOL_1`. Send only the `.ino.bin`. The bicycle must be stationary. The PIN is the value in `OTAPairing.h`.
