#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
set -euo pipefail
tool_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
swiftc -O "$tool_dir/ble_capture.swift" -o "$tool_dir/ble_capture" -framework CoreBluetooth
swiftc -O "$tool_dir/ble_update.swift" -o "$tool_dir/ble_update" -framework CoreBluetooth -framework AppKit
"$tool_dir/ble_update" --self-test
