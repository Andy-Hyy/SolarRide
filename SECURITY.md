# Security

[English](#english) · [中文](#中文)

## English

The current source release is based on V6.5. BLE telemetry is not an authenticated confidential channel. OTA writes require pairing, but a PIN is not a firmware signature. Use your own private PIN, keep USB recovery available, and do updates while parked. Turn the device off when it is not in use.

For a security issue, use GitHub's **Report a vulnerability** option on this repository's Security page if available. If it is unavailable, open a minimal Issue asking the maintainer to enable private reporting, without credentials, exploit instructions, or sensitive logs. Ordinary functional problems belong in the bug-report template. The maintainer does not promise a fixed response time or security support window.

Before publication, the maintainer should enable private vulnerability reporting. No personal email address is exposed here.

## 中文

**安全报告**

当前源码发布基于 V6.5。蓝牙遥测不属于经过认证的保密通道。OTA 写入需要配对，但配对码不等于固件数字签名。请设置私有配对码，保留 USB 恢复条件，停车后再升级；不用时关闭设备。

报告漏洞时，如果仓库 Security 页面提供 **Report a vulnerability**，请使用该私密入口。如果没有，请只开一个简短 Issue 请求开启私密报告，不附凭据、攻击步骤或敏感日志。普通功能问题使用缺陷模板。维护者未承诺固定响应时间或安全支持期限。

发布前维护者应启用 GitHub 私密漏洞报告功能。本文件不公开私人邮箱。
