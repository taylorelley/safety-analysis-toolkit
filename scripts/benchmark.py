# SPDX-License-Identifier: Apache-2.0
"""Record known-answer measurements; missed defects are recorded, never hidden."""
import json,subprocess,time,tempfile,shutil
from pathlib import Path
repo=Path(__file__).resolve().parent.parent
binary=repo/'target/release/safety'
records=[]
for case in json.loads((repo/'tests/benchmarks/manifest.json').read_text())['cases']:
 with tempfile.TemporaryDirectory() as name:
  root=Path(name);file=case['file'];shutil.copyfile(repo/'tests/benchmarks/cases'/file,root/file)
  cpp=file.endswith('cpp')
  (root/'safety.yaml').write_text('version: 1\nproject: {name: known-answer-benchmark}\nbuild: {compile_commands: compile_commands.json}\n')
  (root/'compile_commands.json').write_text(json.dumps([{'directory':name,'file':file,'arguments':['clang++-19' if cpp else 'clang-19','-std=c++17' if cpp else '-std=c11','-c',file]}]))
  args=[binary,'scan',root] if case['engine']=='scan' else [binary,'prove',file,'--path',root,'--engine','esbmc']
  start=time.monotonic();p=subprocess.run(args,capture_output=True,timeout=120);elapsed=time.monotonic()-start
  r=json.loads(p.stdout);detected=bool(r['findings']) or any(x['outcome']=='VIOLATED' for x in r['properties']);incorrect_proof=case['defect'] and r['complete'] and bool(r['properties']) and all(x['outcome']=='PROVEN' for x in r['properties'])
  records.append({**case,'detected':detected,'expected_defect_missed':case['defect'] and not detected,'false_positive':not case['defect'] and detected,'seconds':elapsed,'incomplete':not r['complete'],'successful_proofs':sum(x['outcome']=='PROVEN' for x in r['properties']),'incorrect_proof_classification':incorrect_proof,'exit_code':p.returncode,'native_result':r})
output=repo/'validation/completion/benchmark.json';output.parent.mkdir(parents=True,exist_ok=True)
output.write_text(json.dumps({'version':1,'cases':records,'missed':sum(r['expected_defect_missed'] for r in records),'false_positives':sum(r['false_positive'] for r in records),'incorrect_proofs':sum(r['incorrect_proof_classification'] for r in records)},indent=2)+'\n')
assert not any(r['incorrect_proof_classification'] for r in records),'Unsafe fixture classified proven'
assert not any(r['false_positive'] for r in records),'Unexpected known-safe findings'
print(f'Benchmark: {len(records)} measured cases, {sum(r["expected_defect_missed"] for r in records)} missed, no fabricated results')
