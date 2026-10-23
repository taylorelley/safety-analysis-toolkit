# SPDX-License-Identifier: Apache-2.0
"""Deterministic packaging of original binaries/notices, never external analyzers."""
import hashlib,subprocess,tarfile,tempfile
from pathlib import Path
repo=Path(__file__).resolve().parent.parent
stage=Path(tempfile.mkdtemp(prefix='safety-package-',dir=repo/'build'))
subprocess.run([repo/'scripts/install.sh',stage],check=True,cwd=repo)
output=repo/'build/safety-0.1.0-linux-x86_64.tar.gz'
# tar's metadata is normalized; gzip timestamp is fixed via GzipFile.
import gzip
with output.open('wb') as raw,gzip.GzipFile(filename='',mode='wb',fileobj=raw,mtime=0) as compressed,tarfile.open(fileobj=compressed,mode='w|') as archive:
 for path in sorted(stage.rglob('*')):
  info=archive.gettarinfo(str(path),str(path.relative_to(stage)))
  info.uid=info.gid=0;info.uname=info.gname='';info.mtime=0
  if path.is_file():
   with path.open('rb') as stream:archive.addfile(info,stream)
  else:archive.addfile(info)
(output.with_suffix(output.suffix+'.sha256')).write_text(hashlib.sha256(output.read_bytes()).hexdigest()+'  '+output.name+'\n')
print(output)

import shutil
shutil.rmtree(stage)
