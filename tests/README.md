# Host checks and fixtures

[English](#english) · [中文](#中文)

## English

Run `bash tools/test_host.sh` from the repository root. The tests use the current public headers, not another historical firmware directory. Test program names were shortened for this package; their logic was retained from the earlier regression suite.

`fixtures/mount_direction_calibration.json` describes one actual mounting. `fixtures/device-check-v6.5.json` summarizes historical basic functional observations without private paths, device identifiers or a pairing header. They are not universal settings or a road-validation dataset. Fixtures are CC BY 4.0; original test source is MIT.

## 中文

**主机检查与数据**

在仓库根目录运行 `bash tools/test_host.sh`，检查使用当前公开头文件，不引用其他历史固件目录。整理时简化了文件名，保留已有回归程序逻辑。

安装数据对应一个实物；设备摘要保留已有基础核验，去除了私人路径、设备标识和配对配置。它们不是通用参数或道路验证数据集。数据采用 CC BY 4.0，原创测试程序采用 MIT。
