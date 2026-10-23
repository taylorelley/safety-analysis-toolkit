# Assurance limitations

All outputs are draft evidence for authorized human review. This snapshot cannot
approve deviations, reviews, assurance levels, baselines, compliance or
certification. A clean scan does not demonstrate requirements satisfaction or
absence of defects. The implemented check selection is intentionally limited.

MISRA identifiers in configuration select metadata only. No MISRA guideline has
a validated automated enforcement mapping. The custom check is original project
policy, not a licensed-standard interpretation. No proprietary MISRA text is
distributed. Full guideline inventories, GRP and deviation management are pending. GEP/GCS metadata is emitted only as explicitly incomplete drafts.

DO-178C/DO-278A lifecycle evidence, independent verification, requirements-based
testing, MC/DC assurance completeness, data/control coupling, source/object mapping, target execution,
DO-330 qualification assessment and human sign-off remain outside this snapshot.
Proof outcomes are restricted to the recorded ESBMC model and bound. No proof or compliance conclusion is inferred from missing results.

The engine does not run project scripts, tests or arbitrary compiler commands.
Its analyzer commands request no source fixes. Network sockets and io_uring
creation are denied using seccomp. Environment variables are cleared; no
credentials or proxy settings are inherited. Process groups are killed on timeout
and output is bounded. Trusted tools must not deliberately evade process groups.
This is not a general containment boundary against malicious executables.

Where Linux Landlock ABI >=3 is available, writes are constrained to the current
run directory. Without Landlock the command-level read-only policy and input
change detection remain active, but there is **no filesystem isolation**. Strict
projects should require filesystem isolation in config and execute the whole
process in a separately reviewed container/OS boundary. The current development
host lacks Landlock; the fail-closed strict path is tested, but successful
Landlock enforcement has not yet been validated here.

Workspace files, including untracked/dirty content, are fingerprinted before and
after execution. External system headers, libraries and runtime configuration
are not fully inventoried. Reproducible replay currently requires the same tool
installation and host environment. Hash inventories detect accidental changes;
they do not authenticate evidence against a writer who can replace both files
and hashes. Review decisions are not stored or impersonated by this snapshot.

A SHA inventory binds all generated files except itself. Ordinary operations
create new records and do not overwrite completed records. Loading verifies the
inventory and rejects added, missing, altered or symlinked artifacts.
