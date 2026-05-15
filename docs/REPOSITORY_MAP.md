# Repository Map

This repository is organized as a polished public version of Team 2544X Bolted's software work.

## Current Code

`src/` and `include/` contain the current public PROS/EZ-Template project.

Important files:

- `src/main.cpp`: PROS lifecycle, chassis setup, competition hooks, opcontrol loop.
- `src/driver_assist.cpp`: drive modes, anti-tip, match timer, pit check, driver profiles.
- `src/competition_ui.cpp`: Brain HUD and auton selector.
- `src/path_recorder.cpp`: public guide-only replay module.
- `include/subsystems.hpp`: visible hardware constants.

## Documentation

`docs/` explains the project from different angles:

- `JUDGES_GUIDE.md`: short judge-facing explanation.
- `ARCHITECTURE.md`: how systems fit together.
- `AUTON_REPLAY_CONCEPTS.md`: high-level replay concept.
- `REPLAY_BUILD_GUIDE.md`: partial snippets and implementation ideas.
- `ADAPTATION_CHECKLIST.md`: what another robot must retune.

## Replayability

`replayability/` is the dedicated section for the auton recording idea. It exists so the concept can be studied without turning the private code into a template.

## Legacy

`legacy/` keeps older public files:

- `legacy/push_back/`: older `.v5python` Push Back files.
- `legacy/replay_archive/`: old replay archive.
- `legacy/web/`: old static page.

Legacy files are preserved for history. They are not the current build target.

