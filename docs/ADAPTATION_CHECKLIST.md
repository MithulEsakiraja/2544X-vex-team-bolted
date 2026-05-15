# Adaptation Checklist

This checklist exists so people can learn from the system without pretending this is a universal template.

Before this code can work well on another robot, every item below must be checked and retuned.

## Hardware

- Confirm all drive ports.
- Confirm motor reversals.
- Confirm the IMU port and orientation.
- Measure actual wheel diameter.
- Confirm cartridge RPM.
- Confirm external gear ratio.
- Check that all drive motors report sane temperatures.
- Check that left and right drive sides coast similarly.

## Driver Controls

- Decide whether tank drive is actually the right control style.
- Rebuild the driver profiles around that driver's preference.
- Check that `L1 + UP`, `L1 + DOWN`, `R2`, `R1`, `L2`, and HUD touches do not conflict with mechanisms.
- Add mechanism recording if the robot has intake, clamp, wings, lift, or PTO actions.

## Replay Tuning

- Re-record `QUAL` and `SKILLS` after any code change that affects replay.
- Tune the stopped-command deadband from real driver data.
- Tune heading correction after the IMU is mounted securely.
- Tune distance correction only after motor encoder directions are confirmed.
- Test with fresh battery and lower battery.
- Test on the actual field tiles, not carpet.

## Match Readiness

- Verify the HUD shows the selected slot as ready.
- Verify field connection locks the robot into `MATCH`.
- Verify rumble only happens for match timer warnings.
- Verify SD paths still exist after power cycling.
- Verify RAM fallback only when you accept losing paths after power-off.

## Academic Honesty

If another team uses these ideas, they should be able to explain the concepts, retuning work, and robot-specific changes. If all they did was rename 2544X strings, they did not understand the system.

