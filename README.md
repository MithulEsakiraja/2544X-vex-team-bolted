# 2544X Bolted VEX Code

Public software archive and engineering documentation for Team 2544X Bolted.

This repository has three goals:

1. Keep a buildable 2544X PROS/EZ-Template project available for review and upload.
2. Preserve the older Push Back VEXcode files as open legacy reference material.
3. Explain the replayability/auton recording system as an engineering concept without publishing a one-file copy-paste solution.

## Repository Layout

| Path | Purpose |
| --- | --- |
| `src/` | Current public PROS source code |
| `include/` | Current public PROS headers and bundled library headers |
| `firmware/` | PROS/EZ-Template libraries needed for local builds |
| `bin/` | Uploadable V5 package binaries |
| `docs/` | Architecture, judges guide, and adaptation documentation |
| `replayability/` | Replay/auton recorder design notes and partial snippets |
| `legacy/push_back/` | Older Push Back `.v5python` files kept for public reference |
| `legacy/replay_archive/` | Old replay archive kept as historical material |
| `legacy/web/` | Old web/documentation file |

## Current Public PROS Project

The current project is a C++ PROS/EZ-Template build.

Robot profile:

- Team: `2544X Bolted`
- Drivetrain: 4 motor tank drive
- Left drive ports: `1, 2`
- Right drive ports: `4, 5`, reversed in code
- IMU port: `7`
- Drive cartridge: `600 RPM`
- External gearing: `36` tooth driving gear to `60` tooth driven gear
- Driver modes: `MATCH`, `REC`, `PIT`
- Public replay module: buildable guide-only stub

The public replay module keeps the project uploadable while directing readers to the replayability documentation. The full robot-specific recorder is not included because it depends on private tuning, field testing, and driver workflow.

## Build

Install PROS, then run:

```powershell
pros make
```

The repository also includes package binaries:

- `bin/hot.package.bin`
- `bin/cold.package.bin`

## Upload

Connect the V5 Brain over USB or controller tether:

```powershell
pros upload
```

## Replayability Section

Start here:

- `replayability/README.md`
- `replayability/RECORDER_DESIGN.md`
- `docs/REPLAY_BUILD_GUIDE.md`

These files explain the data model, timing loop, safety checks, SD/RAM storage idea, stopped-side lockout, and correction concepts. They include small snippets, but not the complete private implementation.

## Legacy Push Back Files

The old Push Back VEXcode files are kept in `legacy/push_back/`. They are not the current PROS project, but they are useful for seeing earlier team experiments and strategy history.

See `legacy/README.md` for details.

## Suggested Reading Order

1. `docs/JUDGES_GUIDE.md`
2. `docs/REPOSITORY_MAP.md`
3. `docs/ARCHITECTURE.md`
4. `replayability/README.md`
5. `docs/REPLAY_BUILD_GUIDE.md`
6. `docs/ADAPTATION_CHECKLIST.md`
7. `src/main.cpp`
8. `src/driver_assist.cpp`
9. `src/competition_ui.cpp`

## Reuse Policy

Learn from the concepts. Credit 2544X Bolted. Do not change the branding and present this as a different team's original system.

See `NOTICE.md`.

