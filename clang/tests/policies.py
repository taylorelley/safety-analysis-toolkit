# SPDX-License-Identifier: Apache-2.0
import subprocess,sys
from pathlib import Path
exe,module,base=sys.argv[1:]
for name,expected,valid in [('policies.c',{'safety-unsafe-library-call':2,'safety-repeated-macro-parameter':1},True),('policies.cpp',{'safety-unsafe-library-call':0,'safety-repeated-macro-parameter':0},True),('invalid.c',{'safety-unsafe-library-call':0,'safety-repeated-macro-parameter':0},False)]:
 r=subprocess.run([exe,f'--load={module}','--checks=-*,safety-unsafe-library-call,safety-repeated-macro-parameter','--config={}',str(Path(base)/name),'--','-std=c++17' if name.endswith('cpp') else '-std=c11'],capture_output=True,text=True,timeout=20)
 text=r.stdout+r.stderr
 assert (r.returncode==0)==valid,text
 for check,count in expected.items():assert text.count(f'[{check}]')==count,(name,check,text)
print('Original library/preprocessor policy checks: positive, negative, boundary, macros, C++ and invalid input passed')
