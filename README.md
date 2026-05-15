# 2544X Bolted Override

This is Team 2544X Bolted's public VEX V5 PROS/EZ-Template code release. It is built so judges, teammates, and other builders can understand the structure of our robot software while still keeping the robot-specific replay implementation from becoming a simple rebrand-and-copy template.

The project is buildable and uploadable. It keeps our 2544X Bolted Brain theme, HUD, driver mode structure, drivetrain configuration, pit check flow, and documentation. The public replay source is a safe guide-only module. It compiles, uploads, and shows the 2544X interface, but the working recorded-auton replay logic is explained in documentation rather than shipped as drop-in code.

## Robot Profile

- Drivetrain: 4 motor tank drive
- Drive ports: left `1, 2`; right `4, 5` reversed in code
- IMU port: `7`
- Drive cartridge: `600 RPM`
- External gearing: `36` tooth driving gear to `60` tooth driven gear
- Driver modes: `MATCH`, `REC`, `PIT`
- Public auton replay: guide-only module with learning docs

## What Is Included

- Buildable PROS project files
- 2544X Bolted Brain theme and HUD
- Driver assist system with match, record, and pit modes
- Anti-tip limiting, drive health checks, match timer warnings, and pit motor check
- Auton selector UI for `QUAL`, `SKILLS`, `Q MIR`, and `S MIR`
- Replay concept documentation with partial snippets and tuning notes

## What Is Not Included

The complete working replay recorder is not included in this public version. Recorded-auton code depends heavily on our robot's friction, gearing, encoder polarity, IMU behavior, driver timing, and field testing. Releasing it as a copy-ready file would teach less than explaining the design.

Read `docs/REPLAY_BUILD_GUIDE.md` to understand how the system was designed.

## Build

From this folder:

```powershell
pros make
```

## Upload

Connect the V5 Brain over USB or controller tether:

```powershell
pros upload
```

## Suggested Reading Order

1. `docs/JUDGES_GUIDE.md`
2. `docs/ARCHITECTURE.md`
3. `docs/AUTON_REPLAY_CONCEPTS.md`
4. `docs/REPLAY_BUILD_GUIDE.md`
5. `docs/ADAPTATION_CHECKLIST.md`
6. `src/main.cpp`
7. `src/driver_assist.cpp`
8. `src/competition_ui.cpp`

## Reuse Policy

Learn from the concepts. Credit 2544X Bolted. Do not change the branding and present this as a different team's original system. See `NOTICE.md`.

