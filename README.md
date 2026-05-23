# 2544X Bolted VEX Code

Public software archive and engineering documentation for Team 2544X Bolted.

This repository has three goals:

1. Keep a buildable 2544X PROS/EZ-Template project available for review and upload.
2. Preserve the older Push Back VEXcode files as open legacy reference material.
3. Explain the replayability/auton recording system as an engineering concept while keeping the project tied to 2544X's robot, branding, and workflow.

## Repository Layout

| Path | Purpose |
| --- | --- |
| `src/` | Current public PROS source code |
| `include/` | Current public PROS headers and bundled library headers |
| `firmware/` | PROS/EZ-Template libraries needed for local builds |
| `bin/` | Uploadable V5 package binaries |
| `docs/` | Controls, architecture, judges guide, and adaptation documentation |
| `replayability/` | Replay/auton recorder design notes and teaching snippets |
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
- Replay module: timed tank-drive recorder with QUAL/SKILLS saves, SD metadata indexing, RAM fallback, and mirrored playback

The replay module is intentionally robot-specific. It is useful as a working reference, but it should not be copy-pasted onto another drivetrain without changing ports, gearing, wheel size, tuning, driver workflow, and field testing.

## Where The Code Is

The current robot code is included in this repository.

| File | What to read it for |
| --- | --- |
| `src/main.cpp` | PROS lifecycle, 4 motor tank drive setup, competition callbacks, opcontrol loop |
| `src/path_recorder.cpp` | Record mode, SD/RAM replay storage, metadata files, replay timing, correction, mirrored auton |
| `src/competition_ui.cpp` | Brain HUD, auton selector, QUAL/SKILLS/Q MIR/S MIR selection |
| `src/driver_assist.cpp` | Driver profiles, anti-tip limiting, Pit mode, motor checks, match timer |
| `include/subsystems.hpp` | Hardware profile: ports, 600 RPM drive, wheel size, 36:60 gearing |

The docs explain the design around the code instead of replacing the code.

## Controls

The full control map is in `docs/CONTROLS.md`.

Quick version:

- Tank drive: left stick Y and right stick Y
- `R1`: cycle driver profile
- `L2`: creep drive in Match/Record, pit motor check in Pit
- `L1 + UP`: start recording in Record mode
- `L1 + DOWN`: stop and save recording in Record mode
- `R2`: switch between `QUAL` and `SKILLS` recording slots
- `DOWN + B`: run selected auton in Pit mode only
- Brain HUD: select `QUAL`, `SKILLS`, `Q MIR`, or `S MIR`

## Build

Install PROS, clone the repo, then build from the project folder:

```powershell
git clone https://github.com/MithulEsakiraja/2544X-vex-team-bolted.git
cd 2544X-vex-team-bolted
pros make
```

If the project is already downloaded, just run:

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

If uploading from VS Code, open the repo folder and use the PROS upload button. The project also keeps the latest build artifact at `bin/hot.package.bin`.

## Replayability Section

Start here:

- `replayability/README.md`
- `replayability/RECORDER_DESIGN.md`
- `docs/REPLAY_BUILD_GUIDE.md`

These files explain the data model, timing loop, safety checks, SD/RAM storage, stopped-side lockout, metadata indexing, and correction concepts behind the implementation.

## Legacy Push Back Files

The old Push Back VEXcode files are kept in `legacy/push_back/`. They are not the current PROS project, but they are useful for seeing earlier team experiments and strategy history.

See `legacy/README.md` for details.

## Suggested Reading Order

1. `docs/JUDGES_GUIDE.md`
2. `docs/REPOSITORY_MAP.md`
3. `docs/CONTROLS.md`
4. `docs/ARCHITECTURE.md`
5. `replayability/README.md`
6. `docs/REPLAY_BUILD_GUIDE.md`
7. `docs/ADAPTATION_CHECKLIST.md`
8. `src/main.cpp`
9. `src/driver_assist.cpp`
10. `src/competition_ui.cpp`
11. `src/path_recorder.cpp`

## Reuse Policy

Learn from the concepts. Credit 2544X Bolted. Do not change the branding and present this as a different team's original system.

See `NOTICE.md`.
