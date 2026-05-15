# Auton Replay Concepts

The private 2544X system records what the driver did, then plays it back with small corrections so the robot can repeat the route more consistently.

The public GitHub project keeps a buildable replay stub in `src/path_recorder.cpp`. The complete working recorder is not included. This document explains the concept, and `docs/REPLAY_BUILD_GUIDE.md` shows partial snippets and design steps.

## Recording Idea

In the full robot system, record mode saves timed tank-drive frames. Each frame needs both driver intent and robot feedback:

- elapsed time
- left drive command
- right drive command
- robot heading
- left drive motor position average
- right drive motor position average

The private robot code used a dense recording interval and throttled status text so screen/controller updates did not slow the loop.

## Storage Idea

A competition version should save paths to SD so the replay survives power cycles. RAM can be useful as a fallback while testing, but RAM paths disappear when the Brain turns off.

Typical slot names:

- `QUAL`
- `SKILLS`

The important design rule is to validate files before trusting them. A real system should reject short recordings, bad rows, old formats, and missing sensor data.

## Replay Idea

Replay follows recorded timestamps and sends tank commands back to the left and right sides. Better replay also uses:

- heading correction to resist drift
- distance correction from drive encoder averages
- voltage scaling to reduce battery variation
- anti-tip limiting from IMU pitch/roll
- drivetrain-side calibration for that specific robot

These are concepts, not plug-in constants. Every robot needs its own tuning.

## Stopped-Side Lockout

Tiny joystick leftovers can cause small unwanted rotations. A strong replay system should scrub tiny commands and lock a stopped side to true zero.

That means if the left side was basically stopped during recording, correction code should not be allowed to wake it up during replay. Same for the right side.

## Why The Public Source Uses A Stub

A different robot must retune at least:

- wheel diameter
- gear ratio
- motor cartridge
- motor polarity
- IMU mounting
- encoder polarity
- right/left friction balance
- replay deadband
- heading correction
- distance correction
- driver path timing

The concept is reusable, but a saved replay behavior is a robot fingerprint. The public source is meant to teach the structure without giving away a copy-ready implementation.

