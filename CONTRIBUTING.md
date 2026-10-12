# Contributing

[English](#english) · [中文](#中文)

## English

Small, practical improvements are welcome: clearer assembly instructions, confirmed component substitutions, enclosure refinements, readable firmware changes and carefully described behavior reports.

1. Open an Issue to describe the problem or intended change. Chinese and English are both welcome.
2. Keep a pull request focused. Explain what changed, why, and how you checked it.
3. Run `bash tools/test_host.sh` for detector or OTA changes. Compile the firmware after firmware changes. Update both document languages when meaning changes.
4. Hardware changes should identify the board/model revision, connector polarity, clearances and what was physically checked. Simulation and compilation alone are not road tests.
5. Never commit `OTAPairing.h`, pairing codes, personal paths, device identifiers or unreviewed logs. Check photos for personal information.

By submitting material, you confirm that you have the right to contribute it under the license for that file category. Existing notices stay intact. Credit prior work and document third-party content.

See [Code of conduct](CODE_OF_CONDUCT.md), [Security](SECURITY.md), and [license scope](LICENSE).

## 中文

**参与贡献**

欢迎具体的小改进，包括更清楚的装配说明、核实过的替代器件、外壳改进、容易审阅的代码修改，以及有明确条件的功能反馈。

1. 先开 Issue 说明问题或想改的内容，中英文均可。
2. 每个 PR 尽量聚焦，说明修改内容、原因和核验方法。
3. 修改检测或 OTA 逻辑后运行主机测试；修改固件后重新编译。文档含义有变化时同步两种语言。
4. 硬件修改需注明版本、接头极性、尺寸间隙和实际检查内容；模拟或编译不能代替道路测试。
5. 不提交本地配对配置、配对码、个人路径、设备标识或未经整理的日志；照片也应检查个人信息。

提交材料表示你有权按对应类别的许可贡献这些内容。保留原声明，注明既有工作和第三方内容来源。

参见[交流约定](CODE_OF_CONDUCT.md)、[安全报告](SECURITY.md)和[许可范围](LICENSE)。
