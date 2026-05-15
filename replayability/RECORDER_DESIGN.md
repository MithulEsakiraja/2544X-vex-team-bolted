# Recorder Design

A replayable auton system has two halves:

1. Recording driver intent.
2. Replaying intent with sensor correction.

## Frame Model

Each frame should store enough information to understand both what the driver wanted and what the robot actually did.

Minimum useful frame data:

- elapsed time
- left drive command
- right drive command
- heading
- left drive position
- right drive position

The public snippet is in `snippets/replay_frame.hpp`.

## Recording Loop

A recorder should sample on a consistent interval. It should not write to the Brain every sample because screen writes can slow down the loop.

Important recorder jobs:

- Start from a known sensor reset.
- Capture driver command values.
- Capture heading and motor positions.
- Save to SD if available.
- Keep RAM fallback only for testing.
- Reject files that are too short or in an old format.

## Replay Loop

Replay follows recorded timestamps. At each frame, the robot sends the saved tank commands back to the drive.

The private version also used correction logic:

- heading correction
- encoder distance correction
- low-battery compensation
- anti-tip speed limiting
- stopped-side lockout

These are tuning systems, not universal constants.

## What Makes It Good

A replay system becomes useful when it refuses to pretend bad data is good data. It should fail loudly when:

- the SD file is missing
- the file is too short
- frame times are broken
- old replay formats are detected
- sensor data is missing

