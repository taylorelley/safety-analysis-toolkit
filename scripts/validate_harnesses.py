# SPDX-License-Identifier: Apache-2.0
"""Exercise actual Codex and Pi tool hosts, without a cloud/model dependency."""
import argparse
import json
import os
from pathlib import Path
import select
import subprocess
import time

repo = Path(__file__).resolve().parent.parent
parser = argparse.ArgumentParser()
parser.add_argument('--workspace', type=Path, default=repo/'build/demo')
parser.add_argument('--output', type=Path, default=repo/'validation/completion/harnesses')
args = parser.parse_args()
root, output = args.workspace.resolve(), args.output.resolve()
output.mkdir(parents=True, exist_ok=True)
binary = repo/'target/release/safety'


class Host:
    def __init__(self, command, name, env=None):
        self.stderr = (output/f'{name}-stderr.log').open('wb')
        self.process = subprocess.Popen(command, stdin=subprocess.PIPE,
            stdout=subprocess.PIPE, stderr=self.stderr, bufsize=0, env=env)
        self.pending = b''

    def send(self, value):
        self.process.stdin.write(json.dumps(value).encode()+b'\n')
        self.process.stdin.flush()

    def receive(self, predicate):
        deadline = time.monotonic()+150
        while time.monotonic() < deadline:
            if b'\n' in self.pending:
                line, self.pending = self.pending.split(b'\n', 1)
                value = json.loads(line)
                if predicate(value):
                    return value
                continue
            ready, _, _ = select.select([self.process.stdout], [], [],
                max(0, deadline-time.monotonic()))
            assert ready, 'Agent-host response timeout'
            block = os.read(self.process.stdout.fileno(), 65536)
            assert block, 'Agent host closed stdout'
            self.pending += block
        raise AssertionError('Agent-host response timeout')

    def rpc(self, number, method, params):
        self.send({'id':number, 'method':method, 'params':params})
        value = self.receive(lambda x: x.get('id') == number)
        assert 'error' not in value, value
        return value['result']

    def close(self):
        self.process.stdin.close()
        self.process.terminate()
        try:
            self.process.wait(timeout=10)
        except subprocess.TimeoutExpired:
            self.process.kill()
            self.process.wait()
        self.stderr.close()


def workflow(name, call):
    records = []

    def execute(tool, arguments):
        result = call(tool, arguments)
        assert not result.get('isError'), result
        data = result['structuredContent']
        assert 'status' in data and 'complete' in data, data
        records.append({'tool':tool, 'arguments':arguments, 'result':result})
        (output/f'{name}-results.json').write_text(json.dumps(records, indent=2)+'\n')
        return data

    execute('safety.doctor', {})
    scan = execute('safety.scan', {})
    findings = execute('safety.get_findings', {'run_id':scan['run_id'], 'limit':50})
    rules = {f['rule_id'] for f in findings['findings']}
    assert {'safety-prohibited-goto', 'clang-analyzer-core.NullDereference'} <= rules
    eva = execute('safety.prove', {'source':'harness/navigation_harness.c', 'engine':'frama-c'})
    assert any(p['outcome'] == 'PROVEN' for p in eva['properties'])
    execute('safety.prove', {'source':'harness/navigation_harness.c', 'engine':'esbmc',
        'harness':'harness/navigation_harness.yaml'})
    violated = execute('safety.prove', {'source':'src/controller.cpp', 'engine':'esbmc'})
    assert any(p['outcome'] == 'VIOLATED' for p in violated['properties'])
    detail = execute('safety.get_run', {'run_id':violated['run_id']})
    assert detail['counterexample']['steps'], 'Counterexample unavailable to host'
    trace = execute('safety.trace', {})
    assert trace['finding_count'] > 0
    coverage = execute('safety.coverage', {'profile':'build/test.profdata',
        'binary':'build/test', 'provenance':'build/provenance.json'})
    gaps = execute('safety.get_findings', {'run_id':coverage['run_id'], 'limit':50})
    assert any(f['rule_id'] == 'coverage-gap-mcdc' for f in gaps['findings'])
    execute('safety.report', {'run_id':violated['run_id'], 'format':'markdown'})
    print(f'{name}: all eight MCP operations, native defects/proofs/counterexample/trace/MC/DC validated')


config = 'mcp_servers={safety={command='+json.dumps(str(binary))+',args=["mcp","--path",'+json.dumps(str(root))+'],startup_timeout_sec=30,tool_timeout_sec=120}}'
codex = Host(['codex', '-c', config, 'app-server', '--stdio'], 'codex')
try:
    codex.rpc(1, 'initialize', {'clientInfo':{'name':'safety-validation','version':'1'},
        'capabilities':{'experimentalApi':True}})
    codex.send({'method':'initialized'})
    thread = codex.rpc(2, 'thread/start', {'cwd':str(root),'approvalPolicy':'never',
        'sandbox':'read-only','ephemeral':True})['thread']['id']
    inventory = codex.rpc(3, 'mcpServerStatus/list', {'threadId':thread,'serverName':'safety'})
    (output/'codex-tool-inventory.json').write_text(json.dumps(inventory, indent=2)+'\n')
    counter = [3]

    def codex_call(tool, arguments):
        counter[0] += 1
        return codex.rpc(counter[0], 'mcpServer/tool/call', {'threadId':thread,
            'server':'safety','tool':tool,'arguments':arguments})

    workflow('codex', codex_call)
finally:
    codex.close()

pi_binary = repo/'examples/agent-harnesses/node_modules/.bin/pi'
pi = Host([pi_binary, '--mode','rpc','--no-session','--no-skills',
    '--no-prompt-templates','--no-extensions','-e',
    str(repo/'examples/agent-harnesses/pi-safety.mjs')], 'pi',
    {**os.environ,'SAFETY_BINARY':str(binary),'SAFETY_WORKSPACE':str(root)})
try:
    counter = [0]

    def pi_call(tool, arguments):
        counter[0] += 1
        pi.send({'id':f'validation-{counter[0]}','type':'prompt',
            'message':'/safety-validate '+json.dumps({'tool':tool,'arguments':arguments})})
        event = pi.receive(lambda x: x.get('type') == 'message_end' and
            x.get('message',{}).get('customType') == 'safety-validation')
        return json.loads(event['message']['content'])

    workflow('pi', pi_call)
finally:
    pi.close()
(output/'summary.json').write_text(json.dumps({'status':'passed','complete':True,
    'scope':'Real agent-host MCP interoperability; no model inference or independent human review',
    'hosts':{'codex':subprocess.check_output(['codex','--version'],text=True).strip(),
             'pi':subprocess.check_output([pi_binary,'--version'],text=True,stderr=subprocess.STDOUT).strip()}},indent=2)+'\n')
