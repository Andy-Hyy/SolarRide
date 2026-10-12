#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
set -euo pipefail
repo_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)"
compiler_bin="${CXX:-c++}"
mkdir -p "$repo_dir/build/tests"
for name in brake mount ota; do
  "$compiler_bin" -std=c++17 -O2 -Wall -Wextra \
    -I "$repo_dir/firmware/SolarRider_BLE_OTA_V6_5_20261011" \
    "$repo_dir/tests/test_$name.cpp" -o "$repo_dir/build/tests/test_$name"
  "$repo_dir/build/tests/test_$name"
done
