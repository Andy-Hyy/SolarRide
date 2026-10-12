# Checks and known limits

[中文](../zh-CN/testing.md) · [Home](../../README.md)


## Existing V6.5 build

| Check | Available evidence |
| --- | --- |
| Compilation and USB upload | Completed on 2026-10-11, with upload hash verification |
| Left/right/stop | Confirmed correct on the existing build |
| Basic push/stop lamp behavior | Aggregate positive feedback; individual actions were not precisely annotated |
| Mounted brake warning | Checked on the bicycle after the threshold was recalibrated for the installed tilt |
| Recorded sensor behavior | 8,072 rows over about 180 seconds; all ready, no accumulated read errors or sampling gaps |
| Recorded brake states | Four detector events, all released; not four ground-truth labeled braking actions |
| Road durability, false flashes, different roads | Not measured. The mounted check above is not this |
| Full BLE OTA, interrupted update, rollback | Not physically verified end to end |
| Solar charging, consumption and runtime | Measurements not supplied |
| Enclosures | Screw-fastened approach printed; sliding-frame approach not printed |

This evidence describes the existing privately configured device. A rebuilt public image has not been flashed as part of preparing these files. A sanitized summary is included in [device-check-v6.5.json](../../tests/fixtures/device-check-v6.5.json).

## Run host checks

```sh
bash tools/test_host.sh
python3 tools/check_public.py
```

The regression programs exercise stationary calibration, recovery, sign, short braking, small mounting rotations, synthetic pitch/slope/turning, sensor failures, elapsed-time behavior, and OTA CRC/order/size/timeout/commit cases. These run on a host computer without a bicycle or BLE radio. They do not measure field accuracy, radio compatibility or flash power-loss recovery.

## Practical build check

Label each action before collecting data: stationary, left, right, stop, forward push/stop, return movement, and mounting movement. Verify physical lamp direction and state release. Record attempted actions separately from detector events. Do not compute an accuracy rate without an exact action count and per-action matching.

Mechanical forward/back oscillation can resemble deceleration; sufficiently long synthetic impacts can trigger. The fixed vector, startup stationary window, loose bag motion and road vibration remain material constraints. Further checks should include startup while moving followed by rest, repeatable braking, rough surfaces, slopes and power/connection interruptions, with conditions and results recorded separately.
