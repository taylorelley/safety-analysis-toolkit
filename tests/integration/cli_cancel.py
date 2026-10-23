# SPDX-License-Identifier: Apache-2.0
"""SIGINT cancels a real multi-unit Clang scan and persists incomplete evidence."""
import json
from pathlib import Path
import signal
import subprocess
import sys
import tempfile
import time

with tempfile.TemporaryDirectory() as name:
    root = Path(name)
    (root/'safety.yaml').write_text('version: 1\nproject: {name: cli-cancellation}\nbuild: {compile_commands: compile_commands.json}\n')
    (root/'source.c').write_text('int value(int x) { return x; }\n')
    entries = [{'directory':name,'file':'source.c','arguments':
        ['clang-19','-std=c11',f'-DVARIANT={i}','-c','source.c']} for i in range(1000)]
    (root/'compile_commands.json').write_text(json.dumps(entries))
    process = subprocess.Popen([sys.argv[1],'scan',name],stdout=subprocess.PIPE,stderr=subprocess.PIPE)
    try:
        deadline = time.monotonic()+30
        while not list((root/'.safety/runs').glob('*/raw/clang-0.log')):
            assert process.poll() is None, 'Scan exited before cancellation'
            assert time.monotonic() < deadline, 'Initial native execution timed out'
            time.sleep(0.02)
        process.send_signal(signal.SIGINT)
        out, err = process.communicate(timeout=10)
        assert process.returncode == 3, (process.returncode,out,err)
        run = json.loads(out)
        assert run['status'] == 'incomplete' and run['complete'] is False
        assert run['completeness']['failed'] > 0
        manifest = root/'.safety/runs'/run['id']/'manifest.json'
        assert json.loads(manifest.read_text())['status'] == 'incomplete'
        print('CLI SIGINT: native child cancelled; remaining units incomplete; evidence persisted')
    finally:
        if process.poll() is None:
            process.kill()
            process.wait()
