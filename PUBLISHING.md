# Maintainer publishing guide

[English](#english) · [中文](#中文)

## English

Target repository: https://github.com/Andy-Hyy/SolarRider_box

Upload the **contents of this publication directory** to the repository root. The surrounding collection folder is an archive of original material and includes private device configuration and binaries; it is not the public repository. Review the existing remote files before combining them. These files have been prepared locally, not pushed to GitHub.

### Hardware information and final checks

- [x] Identify CN3163, 4056-TP Type-C management board, XR2981 fixed-5-V boost, 103040 1200 mAh cell, 1 W/6 V panel reference and lighting parts.
- [x] Record the confirmed parallel-panel/shared-cell/management-output-to-boost wiring in the bilingual power guide.
- [x] Include existing bicycle and assembled-controller photographs, STEP renders and PCB1 Gerber/drill export.
- [ ] Confirm actual protection behavior, charge current, simultaneous solar/USB operation and thermal behavior through measurements.
- [ ] Confirm connector pin order, exact enclosure fasteners/print settings, bag capacity and seatpost bracket size for a fully specified replication kit.
- [ ] Review fabrication settings and the selected PCB before ordering; the alternative PCB document is not the confirmed built carrier.

The maintainer has approved the three licenses and publication rights. Final attribution is Andy-Hyy. Supplier screenshots and order details remain private; public references state component specifications without republishing seller imagery.

### Repository setup

1. Use the existing repository name `SolarRider_box` and enable Issues. Optional topics: `esp32-c3`, `bicycle`, `neopixel`, `solar`, `diy`, `open-hardware`, `ble`.
2. Suggested description: “DIY bicycle bag lighting with wireless turn signals, IMU braking indication and solar-assisted charging.”
3. Enable private vulnerability reporting. The supplied workflow compiles and tests source but does not upload firmware to a device or publish binary artifacts.
4. Run `python3 tools/check_public.py` and `bash tools/test_host.sh`. Build the configured firmware and macOS tools. Inspect a Git diff before committing. Confirm no private config, prebuilt image or raw log is tracked.
5. Preview both README languages and model images on GitHub. The workflows have been prepared and locally exercised where possible; a GitHub-hosted run still needs to occur after upload.
6. Publish as an early source release, with the verification scope in the release notes. Do not label it production-ready, fully waterproof, energy-autonomous or road-validated without additional evidence.

Reference: [GitHub repository practices](https://docs.github.com/en/repositories/creating-and-managing-repositories/best-practices-for-repositories).

## 中文

**维护者发布指南**

目标仓库：https://github.com/Andy-Hyy/SolarRider_box

把**本发布目录的内容**放到仓库根目录。外层收集文件夹保留原件，包含现用设备的配对配置和二进制文件，不作为公开仓库整体上传。合并前核对远端已有文件。本次制作仅在本地完成，未推送。

### 硬件资料与最后核验

- [x] 识别 CN3163、4056-TP Type-C 管理板、XR2981 5 V 升压、103040 1200 mAh 电池、1 W/6 V 光伏参考及灯光器件。
- [x] 在双语供电指南记录已确认的光伏并联、电池共接和管理板输出到升压板的接线。
- [x] 收录已有实物照片、STEP 渲染和 PCB1 Gerber/钻孔导出。
- [ ] 测量保护效果、实际限流、双输入同时工作和温升。
- [ ] 完整复刻套件仍需确认接头具体针脚、外壳螺丝/打印参数、车包容量和座管支架尺寸。
- [ ] 下单前核对生产参数及目标板图，另一 PCB 文档不作为已确认实装载板。

维护者已确认三类许可及发布权，署名为 Andy-Hyy。商家截图和订单资料不随公开包上传，公开选型说明记录器件参数。

### 仓库设置

1. 使用现有仓库名 `SolarRider_box` 并开启 Issues。可选主题标签为 `esp32-c3`、`bicycle`、`neopixel`、`solar`、`diy`、`open-hardware`、`ble`。
2. 中文仓库简述可用“开源自行车包灯光小制作”；英文简述见英文部分。
3. 开启私密漏洞报告。工作流只编译和测试，不烧录设备或发布二进制产物。
4. 运行发布检查、主机测试，再编译本地配置的固件和 Mac 工具。提交前看差异，确认未跟踪私有配置、镜像和原始日志。
5. 在 GitHub 预览双语 README 和模型图。可在本地执行的检查已执行，上传后仍需运行 GitHub 工作流。
6. 按早期源码发布说明实际核验范围，无新增依据时不标注生产就绪、整机防水、能量自给或道路验证通过。

参考：[GitHub 仓库实践](https://docs.github.com/en/repositories/creating-and-managing-repositories/best-practices-for-repositories)。
