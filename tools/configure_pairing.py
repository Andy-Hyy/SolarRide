#!/usr/bin/env python3
"""Create a local, ignored BLE PIN header / 创建不进入仓库的本地配对码文件。"""
import argparse
import secrets
from pathlib import Path

parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument('--pin', help='Six digits / 六位数字。Omit to generate securely.')
parser.add_argument('--force', action='store_true', help='Replace existing local config / 替换已有本地配置')
args = parser.parse_args()
if args.pin is not None and (not args.pin.isascii() or not args.pin.isdigit() or len(args.pin) != 6):
    parser.error('--pin must contain six ASCII digits / 必须为六位数字')
pin = args.pin if args.pin is not None else str(secrets.randbelow(900000) + 100000)
root = Path(__file__).resolve().parents[1]
target = root / 'firmware/SolarRider_BLE_OTA_V6_5_20261011/OTAPairing.h'
if target.exists() and not args.force:
    parser.error('Local configuration exists; use --force intentionally / 已有配置，确需替换请加 --force')
text = '#pragma once\n#include <stdint.h>\nstatic constexpr uint32_t solarOTAPairingPIN = ' + str(int(pin)) + ';\n'
if args.force:
    target.write_text(text)
else:
    with target.open('x') as out:
        out.write(text)
target.chmod(0o600)
print('Created local OTAPairing.h. Read it privately when pairing. Do not commit or share it.')
print('已创建本地 OTAPairing.h。配对时自行查看，请勿提交或分享。')
