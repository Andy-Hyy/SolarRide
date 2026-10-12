# Changelog

[English](#english) · [中文](#中文)

## English

### V6.5 source package — 2026-10-12

- Organize the existing working V6.5 firmware, PCB sources, enclosure CAD, macOS tools and bilingual maker documentation.
- Replace the private device pairing configuration with an example and local configuration tool. Do not distribute the existing device's binary images.
- Preserve the detector, light patterns, pin mapping, BLE profile and OTA protocol from the 2026-10-11 source.
- Add build instructions, host regression tests and GitHub contribution templates.

Add component references, maintainer-confirmed CN3163 / 4056-TP / XR2981 power wiring, existing build photographs and PCB1 fabrication sources.

### V6.5 device revision — 2026-10-11

Retain V6.4 detector fixes and swap remote left/right input mapping to GPIO3 / GPIO6. USB programming and basic functional checks were completed on the existing build. This does not establish road reliability or full wireless update verification.

## 中文

**更新记录**

### V6.5 源码发布包 — 2026-10-12

- 整理已有可工作的 V6.5 固件、PCB 源文件、外壳 CAD、Mac 工具和双语制作指南。
- 公开包以示例和本地配置工具替代现用设备的配对配置，不分发现用设备的二进制镜像。
- 保留 2026-10-11 源码的检测逻辑、灯效、引脚、BLE 服务和 OTA 协议。
- 补充编译说明、主机回归测试和 GitHub 协作模板。

补充器件参考、已确认的 CN3163 / 4056-TP / XR2981 接线、实物照片和 PCB1 生产源文件。

### V6.5 实机版本 — 2026-10-11

保留 V6.4 的检测修复，将遥控左/右输入改为 GPIO3 / GPIO6。已有实物完成 USB 烧录和基础功能复测；这不代表道路可靠性或无线升级全过程已验证。
