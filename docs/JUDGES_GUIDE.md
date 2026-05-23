# Judges Guide

This public release shows how 2544X Bolted structures robot code for match reliability, driver usability, and explainable autonomous development.

## What Makes It Different

- The Brain HUD separates match driving, record tools, pit check, and auton selection.
- Field control forces match mode, so record and pit tools cannot be selected during a match.
- The code keeps a branded 2544X Brain theme and clear competition dashboard.
- The driver-assist layer includes anti-tip limiting, drive health checks, match timer warnings, and pit motor checks.
- The auton selector shows the same `QUAL`, `SKILLS`, `Q MIR`, and `S MIR` workflow used by the team.
- The replay module is buildable and robot-specific, with 2544X branding, ports, tuning, and workflow kept visible so the engineering decisions can be reviewed.

## Driver Workflow

The team workflow separates normal match driving from testing tools. Drivers use `MATCH` on the field, `REC` for recording experiments, and `PIT` for quick health checks. When field control is connected, the code locks the robot into `MATCH`.

## Replay Engineering Concept

The replay system was designed around timed tank-drive frames plus feedback correction. The repo explains the concept in `docs/REPLAY_BUILD_GUIDE.md` so judges and students can follow the design choices.

The key engineering problems are:

- wheel slip
- battery variation
- IMU drift
- motor imbalance
- tiny joystick leftover commands
- field tile differences

The public code keeps these ideas visible because that makes the system easier to explain, review, and rebuild honestly.
