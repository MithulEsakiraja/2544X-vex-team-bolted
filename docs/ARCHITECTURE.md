# Architecture

The code is split around match-day responsibilities instead of around one giant `main.cpp`.

## System Map

| File | Job |
| --- | --- |
| `src/main.cpp` | PROS lifecycle, chassis setup, competition hooks, opcontrol loop |
| `include/subsystems.hpp` | Robot hardware profile: ports, wheel size, motor RPM, gear ratio |
| `src/driver_assist.cpp` | Driver profiles, tank output, anti-tip limiting, timer rumble, pit check |
| `src/path_recorder.cpp` | Public guide-only replay stub that preserves build/upload flow |
| `src/competition_ui.cpp` | Brain HUD, auton selector, replay readiness indicators |
| `src/brain_theme.cpp` | 2544X boot screen and visual theme |
| `src/pre_match.cpp` | Pre-match health screen |
| `src/autons.cpp` | EZ-Template drive constants and old/manual auton helpers |

## Lifecycle

`initialize()` sets up the theme, drive constants, recorder, driver assist, HUD, and EZ chassis.

`competition_initialize()` shows the pre-match checklist and refreshes the auton selector.

`autonomous()` resets drivetrain sensors and calls the selected HUD auton. In this public release, replay calls show a guide notice instead of running the private recorded path implementation.

`opcontrol()` runs driver assist every loop. Record mode still routes through the replay module so the public project demonstrates the control flow without shipping the complete replay recorder.

## Design Choices

The code intentionally keeps hardware assumptions visible instead of hiding them in magic helpers. That makes it easier for 2544X to debug at events, and harder for another robot to copy without understanding why the values exist.

Examples:

- `kDriveMotorRpm` and `kDriveExternalRatio` are explicit in `include/subsystems.hpp`.
- Replay concepts are documented separately so teams can understand the design before writing their own implementation.
- The HUD shows storage and readiness instead of pretending every auton is always safe.
