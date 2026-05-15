# Safety And Tuning

Replay systems can look simple and still fail in subtle ways. These are the checks that matter.

## Stopped-Side Lockout

Tiny joystick leftovers can wake up one side of the drive during replay. Treat very small commands as zero.

If a side was not intentionally moving, correction logic should not be allowed to move it.

## Sensor Reset

Before replay starts:

- reset drive sensors
- reset PID targets
- reset heading or odom state
- set drive brake mode intentionally

## Voltage Behavior

Do not blindly slow down replay because the battery is fresh. If voltage compensation exists, it should avoid reducing a recorded command below what the driver actually drove.

## File Validation

Never trust a file just because it exists.

Check:

- minimum frame count
- valid timestamp ordering
- expected column count
- sane command range
- usable sensor data

## Field Tuning

Tune on the surface and battery conditions where the robot will compete. Carpet, field tile, fresh battery, and low battery can all change replay behavior.

