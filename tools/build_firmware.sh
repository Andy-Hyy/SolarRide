#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
set -euo pipefail
repo_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)"
sketch_dir="$repo_dir/firmware/SolarRider_BLE_OTA_V6_5_20261011"
cli_bin="${ARDUINO_CLI:-arduino-cli}"
if [[ ! -f "$sketch_dir/OTAPairing.h" ]]; then
  echo "Run python3 tools/configure_pairing.py first / 请先生成本地配对配置" >&2
  exit 1
fi
mkdir -p "$repo_dir/build/firmware"
"$cli_bin" compile --fqbn esp32:esp32:AirM2M_CORE_ESP32C3 \
  --output-dir "$repo_dir/build/firmware" "$sketch_dir"
