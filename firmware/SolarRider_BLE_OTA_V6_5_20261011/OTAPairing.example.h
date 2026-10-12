// SPDX-License-Identifier: MIT
#pragma once
#include <stdint.h>
// Generate OTAPairing.h with tools/configure_pairing.py, or copy this example,
// replace 0 with a private six-digit PIN and remove the #error below.
// 使用配置工具生成本地文件；手动复制时设置六位私有码并删除下方 #error。
#error "Configure a private BLE pairing PIN before building / 请先设置私有蓝牙配对码"
static constexpr uint32_t solarOTAPairingPIN = 0;
