# SPDX-License-Identifier: Apache-2.0
"""Review the locked Rust graph against this release's permissive-license policy."""
import json,re,subprocess,shutil,sys
from pathlib import Path
repo=Path(__file__).resolve().parent.parent
out=Path(sys.argv[1]) if len(sys.argv)>1 else repo/'build/license-inventory'
out.mkdir(parents=True,exist_ok=True)
metadata=json.loads(subprocess.check_output(['cargo','metadata','--locked','--format-version','1','--filter-platform','x86_64-unknown-linux-gnu'],cwd=repo))
allowed={'MIT','Apache-2.0','BSD-2-Clause','BSD-3-Clause','ISC','Unicode-3.0','Unicode-DFS-2016','Zlib','Unlicense','CC0-1.0','0BSD','BSL-1.0','LLVM-exception','OR','AND','WITH'}
def select_license(expression):
 tokens=re.findall(r'\(|\)|[A-Za-z0-9.+-]+',expression);index=0
 def atom():
  nonlocal index
  if tokens[index]=='(':
   index+=1;choice=either();assert tokens[index]==')';index+=1;return choice
  name=tokens[index];index+=1
  if index<len(tokens) and tokens[index]=='WITH':
   index+=1;exception=tokens[index];index+=1
   return name+' WITH '+exception if name in allowed and exception=='LLVM-exception' else None
  return name if name in allowed else None
 def both():
  nonlocal index
  choice=atom()
  while index<len(tokens) and tokens[index]=='AND':
   index+=1;right=atom();choice=f'{choice} AND {right}' if choice and right else None
  return choice
 def either():
  nonlocal index
  choice=both()
  while index<len(tokens) and tokens[index]=='OR':
   index+=1;right=both();choice=choice or right
  return choice
 choice=either();assert index==len(tokens);return choice
assert select_license('GPL-3.0 OR MIT')=='MIT'
assert select_license('MIT AND GPL-3.0') is None
nodes={node['id']:node for node in metadata['resolve']['nodes']}
active=set();pending=[metadata['resolve']['root']]
while pending:
 key=pending.pop()
 if key in active:continue
 active.add(key);pending.extend(dep['pkg'] for dep in nodes[key]['deps'])
records=[]
for package in metadata['packages']:
 if package['name']=='safety' or package['id'] not in active:continue
 expression=package.get('license') or ''
 try:chosen=select_license(expression.replace("/"," OR ")) if expression else None
 except AssertionError as error:raise AssertionError((package['name'],expression)) from error
 assert chosen,(package['name'],expression,'manual dependency review required')
 source=Path(package['manifest_path']).parent
 files=sorted([p for p in source.rglob('*') if p.is_file() and (p.name.lower().startswith(('license','licence','copying','notice','copyright')) or any(part.lower() in ['licenses','licences'] for part in p.relative_to(source).parts[:-1]))])
 if package.get('license_file'):
  declared=source/package['license_file']
  if declared not in files:files.append(declared)
 assert files,(package['name'],'no distributable license text found')
 destination=out/f"{package['name']}-{package['version']}"
 destination.mkdir(exist_ok=True)
 for path in files:
  target=destination/path.relative_to(source);target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(path,target)
 records.append({'name':package['name'],'version':package['version'],'license':expression,'selected_license':chosen,'review':'permissive SPDX policy passed; retain bundled notices','license_files':sorted(str(p.relative_to(out)) for p in destination.rglob("*") if p.is_file())})
(out/'inventory.json').write_text(json.dumps({'version':1,'policy':'Apache-2.0 original code; retain all dependency notices; no analyzer redistribution','packages':records},indent=2)+'\n')
print(f"Reviewed {len(records)} locked Rust packages and collected their license/notice texts")
