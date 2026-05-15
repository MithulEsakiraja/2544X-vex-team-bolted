# Replay Build Guide

This document explains the recorded-auton replay concept used by 2544X Bolted. It intentionally does not provide a complete copy-paste implementation. A good replay system has to be tuned on the robot that will run it.

## Goal

The driver records a route once, then the robot replays the same tank-drive commands during autonomous. The replay is improved with sensor feedback so small battery, heading, and drivetrain differences do not ruin the path.

## Core Data Shape

The real system stores a frame for each moment in the recording. The important idea is to save both driver intent and robot feedback.

```cpp
struct RecordedPathFrame {
  int time_ms;
  int left;
  int right;
  double heading;
  double left_position;
  double right_position;
};
```

This is not enough by itself. A full implementation also needs validation, file loading, file saving, replay timing, safety stops, and tuning constants.

## Recording Concept

Record at a fixed interval while the driver moves. The driver command is saved with the robot's heading and drive encoder position.

```cpp
if (now - last_sample_time >= record_interval_ms) {
  push_frame(elapsed_ms,
             left_driver_output,
             right_driver_output,
             current_heading,
             left_drive_position,
             right_drive_position);
}
```

Do not print to the Brain or controller every frame. Screen writes can slow the loop down and make the replay miss pieces of the path.

## Storage Concept

Use SD for paths that need to survive power cycles. Use RAM only as a fallback while testing.

```cpp
// Example path names, not the full file system implementation.
"/usd/qual_replay.csv"
"/usd/skills_replay.csv"
```

A serious version should reject old formats, short recordings, corrupted lines, and files without enough frames.

## Replay Timing Concept

Replay follows the recorded timestamps. The loop waits until the next frame time, then sends the saved left and right commands.

```cpp
for (const RecordedPathFrame& frame : frames) {
  while (millis() - replay_start < frame.time_ms) {
    delay(1);
  }

  move_left(frame.left);
  move_right(frame.right);
}
```

That simple version will drift. The real work is in the correction and safety rules.

## Correction Concept

Heading correction compares the recorded heading to the current heading.

```cpp
double error = target_heading - current_heading;
int correction = clamp(round(error * heading_kp), -max_turn, max_turn);
left_target -= correction;
right_target += correction;
```

Distance correction compares recorded motor position to current motor position.

```cpp
double error = target_position - current_position;
int correction = clamp(round(error * distance_kp), -max_distance, max_distance);
```

These values must be tuned. If the constants are too low, replay drifts. If they are too high, the robot fights itself.

## Stopped-Side Lockout

Tiny controller leftovers can make the robot rotate when it should sit still. The 2544X system treats very small commands as zero and prevents correction code from waking up a stopped side.

```cpp
int scrub_still_command(int input) {
  return abs(input) < still_deadband ? 0 : input;
}
```

The idea is simple: if a side was not intentionally moving, force that side to zero during replay.

## What You Must Tune Yourself

- Drive motor ports and reversals
- Motor cartridge RPM
- External gear ratio
- Wheel diameter
- IMU port and mounting direction
- Encoder polarity
- Still-command deadband
- Heading correction gain
- Distance correction gain
- Battery compensation
- Anti-tip speed limits
- Driver path timing

## Why The Public Source Uses A Stub

The public `src/path_recorder.cpp` keeps this project buildable and shows where the replay system plugs into the robot code. The complete implementation is not included because a replay system copied without tuning is unreliable and does not show real engineering understanding.

Use this guide to build your own version, test it, and explain the choices you made.
