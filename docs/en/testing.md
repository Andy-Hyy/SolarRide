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

This is what I saw on the box I ride. I did not reflash a new export just to write these notes.

## After you build one

Label each action before collecting data: stationary, left, right, stop, forward push/stop, return movement, and mounting movement. Verify physical lamp direction and state release. Record attempted actions separately from detector events. Do not compute an accuracy rate without an exact action count and per-action matching.

Mechanical forward/back oscillation can resemble deceleration; sufficiently long synthetic impacts can trigger. The fixed vector, startup stationary window, loose bag motion and road vibration remain material constraints. Further checks should include startup while moving followed by rest, repeatable braking, rough surfaces, slopes and power/connection interruptions, with conditions and results recorded separately.
