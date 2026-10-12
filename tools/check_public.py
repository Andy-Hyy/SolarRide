#!/usr/bin/env python3
"""Check public source contents and local Markdown links / 检查公开源码和文档链接。"""
from pathlib import Path
from urllib.parse import unquote,urlsplit
import re, subprocess, sys

root=Path(__file__).resolve().parents[1]
ignored={'build','logs','.git','__pycache__','.venv'}
problems=[]
try:
    is_repo=subprocess.run(['git','rev-parse','--show-toplevel'],cwd=root,capture_output=True,text=True)
    exact_repo=is_repo.returncode==0 and Path(is_repo.stdout.strip()).resolve()==root
except FileNotFoundError:
    exact_repo=False
if exact_repo:
    names=subprocess.check_output(['git','ls-files','-z'],cwd=root).decode().split('\0')
    files=[root/n for n in names if n and (root/n).is_file()]
else:
    files=[p for p in root.rglob('*') if p.is_file() and not any(x in ignored for x in p.relative_to(root).parts)]
    # Local build config is deliberately ignored; archives must omit it.
    files=[p for p in files if p.name!='OTAPairing.h' and p.name not in {'ble_update','ble_capture'}]
for p in files:
    rel=str(p.relative_to(root))
    if p.name=='OTAPairing.h' or p.suffix in {'.bin','.elf','.log'}:
        problems.append(rel+': private config/generated binary/raw log must not be published')
    if p.suffix in {'.md','.txt','.py','.h','.ino','.cpp','.swift','.sh','.json','.yml'}:
        t=p.read_text(errors='replace')
        if re.search(r'/Users/[A-Za-z0-9_.-]+/',t):
            problems.append(rel+': local personal filesystem path')
        if re.search(r'solarOTAPairingPIN\s*=\s*[1-9]\d{5}',t):
            problems.append(rel+': hard-coded six-digit device PIN')
        if p.suffix=='.md':
            # Fenced code can contain example paths, not document hyperlinks.
            t=re.sub(r'```.*?```','',t,flags=re.S)
            for link in re.findall(r'\]\(([^)]+)\)',t):
                dest=link.split(' "',1)[0].strip('<>')
                if dest.startswith('#') or urlsplit(dest).scheme:continue
                path=unquote(dest.split('#',1)[0])
                if path and not (p.parent/path).exists():
                    problems.append(rel+': broken local link '+dest)
for locale in ['en','zh-CN']:
    guide_names={p.stem for p in (root/'docs'/locale).glob('*.md')}
    other={p.stem for p in (root/'docs'/('zh-CN' if locale=='en' else 'en')).glob('*.md')}
    if guide_names!=other:problems.append('English/Chinese guide names differ')
if problems:
    print('\n'.join(problems));sys.exit(1)
print(f'PASS: {len(files)} public files, paired guides, local links and private-configuration checks.')
print('通过：公开文件、双语指南、内部链接及私有配置检查。')
