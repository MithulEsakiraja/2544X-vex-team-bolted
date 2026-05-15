# Replayability System

This folder explains the recorded-auton replay concept used by 2544X Bolted.

The public repository does not include the private full replay implementation. Instead, it documents how the system works and gives short snippets that show the core ideas.

## Why This Is Separate

Replay code is not portable by copy-paste. It depends on:

- drivetrain friction
- motor direction
- wheel diameter
- external gearing
- IMU placement
- battery behavior
- driver path timing
- field surface

Publishing the exact private implementation would make the repo easier to clone but worse for learning.

## Files

- `RECORDER_DESIGN.md`: design of the recorder and replay loop.
- `SAFETY_AND_TUNING.md`: checks that make replay safer and more repeatable.
- `snippets/replay_frame.hpp`: small data-shape snippet.
- `snippets/replay_timing.cpp`: short timing-loop snippet.

## Where It Connects To Code

The public project keeps `src/path_recorder.cpp` as a guide-only stub. It compiles and uploads, but it points users back here for the actual concepts.

