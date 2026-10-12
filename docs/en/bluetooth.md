# Bluetooth data and maintenance

[中文](../zh-CN/bluetooth.md) · [Home](../../README.md)


## Read-only telemetry

The device advertises as `SolarRider`. Service `6E400001-B5A3-F393-E0A9-E50E24DCCA9E` provides notifications on `6E400003-B5A3-F393-E0A9-E50E24DCCA9E`. Rows are ASCII CSV split into 20-byte notifications; clients must reassemble complete newline-terminated rows. BLE sends up to 10 rows/s and may drop intermediate rows. USB targets up to 50 rows/s. Neither transport is a persistent onboard log.

On macOS, install Xcode Command Line Tools if needed, then run:

```sh
bash tools/macos/build_tools.sh
mkdir -p logs
tools/macos/ble_capture 300 > logs/capture.txt
```

The seconds argument controls the capture window, including connection time. Allow Bluetooth access when macOS asks. Close other connected clients. The capture tool reconnects after disconnection and ignores incomplete rows. It selects the first matching `SolarRider`; use it near one intended powered-on device.

The V6.5 row has 26 columns: `t_us,seq,loop_dt_us,ax,ay,az,gx,gy,gz,gravity_x,gravity_y,gravity_z,forward_raw,forward_filtered,old_diff_y,old_hits,old_brake,ready,brake,events,mode,dropped,cal_reason,acc_std_max,gyro_std_max,gravity_error`. Acceleration is m/s²; gyro is rad/s. Calibration reasons are 0 ready, 1 collecting, 2 acceleration motion, 3 gyro motion, 4 invalid/read failure, 5 gap, 6 invalid magnitude. `dropped` counts USB debug writes that could not fit, not a guaranteed BLE loss counter.

## BLE firmware updates

Run `tools/macos/ble_update` to choose a locally compiled application `.bin`. Keep the device parked and still, wait until calibrated, close the capture client and allow roughly eight seconds after startup before trying an update. Use the private PIN from your own local header when asked. The tool is interactive on macOS and includes Chinese prompts; the steps here describe its operation in English.

For an offline image check, use:

```sh
tools/macos/ble_update --validate build/firmware/SolarRider_BLE_OTA_V6_5_20261011.ino.bin
```

OTA service `0x8018` uses firmware `0x8020`, progress `0x8021`, command `0x8022`, and information `0x8023`. The transfer uses 4 KiB sectors, CRC16 and ordered packets. The receiver checks the C3 application identity, size and OTA marker. CRC and image hashes detect integrity errors; they are not publisher signatures.

The updater waits for restart and checks the image identity and boot state. Receiver implementation and host checks are included, but a complete real BLE transfer/restart/recovery has not been verified. Generic file import or profile compatibility does not establish iPhone compatibility. Future updates must preserve `SolarRider_OTA_PROTOCOL_1` and the OTA receiver. Rollback hooks depend on the core/bootloader configuration and are not a guarantee of recovery; keep USB access available.
