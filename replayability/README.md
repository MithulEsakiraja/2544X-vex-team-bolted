# Replayability System

This folder explains the recorded-auton replay concept used by 2544X Bolted.

The repository includes the branded 2544X replay implementation and documents how the system works. The goal is learning from the engineering decisions, not cloning the branding and hoping it works on a different robot.

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

Publishing robot code without context makes it easy to misuse. This section gives the context: why the recorder stores each field, how the timing loop works, and where tuning lives.

## Files

- `RECORDER_DESIGN.md`: design of the recorder and replay loop.
- `SAFETY_AND_TUNING.md`: checks that make replay safer and more repeatable.
- `snippets/replay_frame.hpp`: small data-shape snippet.
- `snippets/replay_timing.cpp`: short timing-loop snippet.

## Where It Connects To Code

The public project keeps the active implementation in `src/path_recorder.cpp`, but the values are tuned for 2544X. Read this section before changing ports, drive ratio, replay interval, deadbands, or correction constants.
