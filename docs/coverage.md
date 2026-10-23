# Coverage import

`coverage import` invokes LLVM 19 `llvm-cov export` over an existing binary and
merged profile. It never executes the binary, builds the project or merges raw
profiles automatically. It accepts LLVM coverage JSON version 2.0.1, preserves
source/branch/region/MC/DC records, reports measured gaps and retains raw output.

```sh
safety coverage import --profile build/test.profdata --binary build/test \
  --provenance build/coverage-provenance.json
```

Provenance is user-supplied JSON with these fields:

```json
{
  "version": 1,
  "binary_sha256": "64-hex-digits",
  "profile_sha256": "64-hex-digits",
  "sources": {"src/decision.c": "64-hex-digits"},
  "compile_commands_sha256": "64-hex-digits",
  "execution_environment": "host",
  "build_variant": "host-debug",
  "instrumentation_warnings": [],
  "exclusions": [],
  "test_ids": ["TST-DEC-1"],
  "requirement_ids": ["REQ-DEC-1"]
}
```

Files and supplied hashes are verified. Host/target labels and test/requirement
IDs are checked against the external trace manifest, including typed nodes and
requirement/test reachability. Their execution meaning is a supplied assertion,
not independently authenticated. Per-condition missing MC/DC independence pairs
retain the decision source location and associated requirement/test IDs. Missing/inconsistent provenance, warnings,
exclusions, missing units, corrupt profiles or decisions without MC/DC
measurement leave the import incomplete. All compilation variants remain in
scope; this initial importer does not establish per-variant instrumentation
identity from binary data alone.

Measured coverage gaps are findings, even when provenance is valid. Missing
provenance does not hide the measurements: it makes their scope incomplete.
Compiler instrumentation diagnostics must be preserved in provenance because a
binary export alone cannot reveal every excluded expression or instrumentation
limit. Supplying an empty warnings array does not prove no warning occurred.

Coverage is not certification evidence completeness. Requirement/test links,
requirements-based test adequacy, infeasible combinations, dead/defensive code,
data/control coupling, source/object mapping, target execution and qualification
assessment need investigation and review. No arbitrary tests are generated to
increase a percentage. See the real integration fixture for a known 50% MC/DC
gap, missing-provenance case and corrupt-profile rejection.
