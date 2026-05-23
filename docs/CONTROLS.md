# Controls

This is the current 2544X Bolted driver and Brain control map.

## Controller

| Control | Mode | Action |
| --- | --- | --- |
| Left stick Y | Match, Record, Pit | Left tank drive |
| Right stick Y | Match, Record, Pit | Right tank drive |
| `R1` | All driver modes | Cycle driver profile: `FULL`, `PRECISION`, `DEFENSE`, `SKILLS` |
| `L2` | Match, Record | Creep/slow drive while held |
| `L2` | Pit | Run pit motor check |
| `L1 + UP` | Record only | Start recording the selected replay slot |
| `L1 + DOWN` | Record only | Stop recording and save the selected replay slot |
| `R2` | Record only | Toggle record slot between `QUAL` and `SKILLS` |
| `DOWN + B` | Pit only | Run the currently selected Brain HUD auton for testing |

## Brain HUD

| Brain control | Action |
| --- | --- |
| `QUAL` | Select normal qualification replay |
| `SKILLS` | Select normal skills replay |
| `Q MIR` | Select mirrored qualification replay |
| `S MIR` | Select mirrored skills replay |
| Bottom `MATCH` button | Normal driver mode |
| Bottom `REC` button | Recording mode |
| Bottom `PIT` button | Pit check and pit auton test mode |

## Match Safety

When field control/VEXnet is connected, the robot forces `MATCH` mode.

In `MATCH` mode:

- The controller only drives the robot and changes driver profiles.
- Record controls are unavailable.
- Pit test controls are unavailable.
- Manual auton replay is unavailable.
- Autonomous only runs from the official PROS `autonomous()` callback using the selected Brain HUD auton.

## Record Workflow

1. Tap `REC` on the Brain.
2. Select the slot with `R2`: `QUAL` or `SKILLS`.
3. Hold `L1` and press `UP` to start recording.
4. Drive the path with tank drive.
5. Hold `L1` and press `DOWN` to stop and save.
6. The path saves to SD when available, with RAM as fallback.

## Pit Testing Workflow

1. Tap `PIT` on the Brain.
2. Press `L2` to test each drive motor and show RPM, temperature, and current.
3. Select an auton on the Brain HUD.
4. Hold `DOWN` and press `B` to replay that selected auton in Pit mode.

Pit auton testing is locked out when field control is connected.
