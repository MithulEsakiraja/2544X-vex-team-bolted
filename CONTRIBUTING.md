# Contributing

This repository is maintained as a public team archive and engineering showcase.

## Rules For Changes

- Keep the current PROS project buildable.
- Keep 2544X Bolted branding intact.
- Do not publish the private full replay recorder in public source.
- Put older experiments under `legacy/`.
- Put explanatory material under `docs/` or `replayability/`.
- Avoid game-specific wording unless the file is clearly marked as legacy.

## Before Committing

Run:

```powershell
pros make
```

Then check:

- `README.md` still describes the current repository accurately.
- `src/path_recorder.cpp` remains the public guide-only module.
- `legacy/push_back/` still contains old `.v5python` files, not current PROS code.
- No generated object files or ELF files are staged.

## Documentation Style

- Be specific.
- Use plain technical language.
- Explain why a feature exists, not only what it does.
- Keep snippets short when describing private robot-specific systems.

