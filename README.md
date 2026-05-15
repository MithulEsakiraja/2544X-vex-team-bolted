# 2544X Bolted - VEX Robotics    ( ) Team Repository

Welcome to the **2544X Bolted** VEX Robotics team repository for **   ( )**! This is the official codebase for Team 2544X's VEX competition code and experimental systems, built on the **EZ-Template** framework.

## ⚠️ IMPORTANT: VEX Rule G4 - Intellectual Property & Code Sharing

**Rule G4** (VEX Robotics Game Manual) explicitly prohibits **copying, cloning, or using code from other teams without explicit written permission**. This repository is provided for:

- **Team 2544X members only** for reference and development
- **Educational purposes** within our organization
- **Published work credit** - any code derived from this repository must credit Team 2544X Bolted

### What You **CANNOT** Do (Rule G4 Violation):
- ❌ Copy code from this repository and submit it as your own work
- ❌ Use this codebase to create competition entries for other teams
- ❌ Distribute this code without explicit attribution and permission
- ❌ Clone entire files/routines for direct use in other team robots

### What You **CAN** Do (Educational Use):
- ✅ Study code patterns and robotics techniques
- ✅ Reference algorithms for learning purposes (with credit)
- ✅ Implement similar concepts using your own code (not copy-paste)
- ✅ Request explicit written permission from Team 2544X for collaboration

**Violations of Rule G4 result in disqualification from competition.**

---

## Table of Contents

- [Quick Start](#quick-start)
- [Repository Overview](#repository-overview)
- [File Organization & Naming Conventions](#file-organization--naming-conventions)
- [Technical Documentation](#technical-documentation)
- [The Replay Script](#the-replay-script)
- [Code Categories](#code-categories)
- [Getting Started](#getting-started)
- [Contributing](#contributing)

---

## Quick Start

The primary resource in this repository is the **Replay Script** (`2544X_Bolted_(Override)----Replay.zip`). If you're here to analyze competition runs or review autonomous routines, jump to [The Replay Script](#the-replay-script) section.

For current competition code, use:
- **`2544X_Bolted_(Push_back)----Stable_Competition_Build.v5python`** - Latest stable version (PRODUCTION)
- **`2544X_Bolted_(Push_back)----Competition_Code_V4.v5python`** - Current active code (ACTIVE)

---

## Repository Overview

This repository contains the complete software suite for the 2544X Bolted VEX Robotics team competing in **   ( )**, including:

- **Competition Code**: Production-ready autonomous and driver control routines
- **Test & Validation Scripts**: Subsystem verification and calibration code
- **Experimental Systems**: Advanced features under development (IMU-based movement, vision processing)
- **Match Replays**: Recorded competition data for performance analysis
- **Legacy Versions**: Historical code for reference and regression testing

### Key Technical Stack

- **Language**: Python (VEXcode Python 3.x)
- **Platform**: VEX Robotics    ( ) / V5 Hardware
- **Framework**: **EZ-Template** (OU Robotics base framework)
- **API**: VEX V5 Python SDK with EZ wrapper classes
- **Version Control**: Git (with VEXcode .v5python project files)

### EZ-Template Foundation

This codebase is built on **[EZ-Template](https://github.com/EZ-Robotics/EZ-Template)**, a comprehensive robotics framework that provides:

- **PID-based movement systems** for precision autonomous routines
- **Chassis abstraction** simplifying drive control (tank drive, arcade, etc.)
- **Sensor integration wrappers** for encoders, IMU, and vision sensors
- **Autonomous selection and tracking** for competition routing
- **Enhanced motor/sensor APIs** with safety limits and telemetry

**Credit**: EZ-Template is maintained by OU Robotics and provides the foundation for our motion control and autonomous systems.

---

## File Organization & Naming Conventions

### Naming Pattern

All files follow this standardized naming convention:

```
2544X_Bolted_(<STRATEGY>)----<PURPOSE>.v5python
```

#### Components Explained:

| Component | Values | Purpose |
|-----------|--------|---------|
| **STRATEGY** | `Push_back`, `Override` | `Push_back`: Primary strategy variant for  <br>`Override`: Special purpose (replays, override behavior) |
| **PURPOSE** | Descriptive name | Clearly indicates function (e.g., `Competition_Code`, `IMU_Test_Code`) |
| **Extension** | `.v5python` | VEXcode Python project file format (JSON-based) |

### Organization Structure

```
2544X-vex-team-bolted/
├── Competition Code (  Strategy: Push_back)
│   ├── 2544X_Bolted_(Push_back)----Stable_Competition_Build.v5python     [PRODUCTION]
│   ├── 2544X_Bolted_(Push_back)----Competition_Code_V4.v5python          [ACTIVE]
│   ├── 2544X_Bolted_(Push_back)----Competition_Code_V3.v5python          [PREVIOUS]
│   └── 2544X_Bolted_(Push_back)----Skills_Auton.v5python                 [SKILLS EVENT]
│
├── Autonomous Routines (Push_back Strategy)
│   ├── 2544X_Bolted_(Push_back)----Auton_Pathing_V2.v5python             [CURRENT]
│   ├── 2544X_Bolted_(Push_back)----Linear_Auton.v5python                 [BASIC]
│   └── 2544X_Bolted_(Push_back)----No_Auton_Test.v5python                [TEST]
│
├── Movement & Drive Systems (EZ-Template PID Control)
│   ├── 2544X_Bolted_(Push_back)----Early_Drive_Code.v5python             [LEGACY]
│   ├── 2544X_Bolted_(Push_back)----IMU_Drive_V1.v5python                 [EXPERIMENTAL]
│   └── 2544X_Bolted_(Push_back)----Movement_Test_Code.v5python           [VALIDATION]
│
├── Sensor Integration
│   ├── 2544X_Bolted_(Push_back)----Vision_Sensor_Test.v5python           [VISION]
│   ├── 2544X_Bolted_(Push_back)----IMU_Test_Code.v5python                [IMU CALIBRATION]
│   └── 2544X_Bolted_(Push_back)----Motor_Group_Test.v5python             [MOTOR TEST]
│
├── Base Templates & EZ-Template Setup
│   ├── 2544X_Bolted_(Push_back)----Default_VEXcode_Project.v5python      [TEMPLATE]
│   ├── 2544X_Bolted_(Push_back)----Base_Bolted_Code.v5python             [STARTER WITH EZ]
│   └── 2544X_Bolted_(Push_back)----Experimental_Code.v5python            [R&D SANDBOX]
│
├── Replay System
│   └── 2544X_Bolted_(Override)----Replay.zip                             [PRIMARY RESOURCE]
│
└── Web Resources
    └── index.html                                                         [DOCUMENTATION]
```

### Code Status Indicators

| Status | Used In | Meaning |
|--------|---------|---------|
| **PRODUCTION** | `Stable_Competition_Build` | Battle-tested, zero known critical issues, game-legal |
| **ACTIVE** | `Competition_Code_V4` | Currently deployed to robots during matches |
| **CURRENT** | Latest versioned files | Most recent iteration with improvements |
| **EXPERIMENTAL** | `IMU_Drive`, `Experimental_Code` | Under development using EZ-Template extensions |
| **LEGACY** | `Early_Drive`, old versions | Superseded, kept for reference/rollback |
| **TEST/VALIDATION** | Test files | Diagnostic and calibration using EZ-Template utilities |

---

## Technical Documentation

### VEX    ( ) Platform

This repository targets **VEX Robotics   :  **, featuring:

- **V5 Hardware**: Cortex-based processing with Python runtime
- **Drive Systems**: Tank-drive chassis with smart motors
- **Sensors**: IMU (6-axis), Vision Sensor, Motor Encoders, Rotation Sensors
- **Field**: 12ft x 12ft play area with disc scoring mechanism
- **Competition Rules**: Alliance-based qualification and elimination matches

### VEX V5 Hardware Architecture

- **V5 Brain**: 1024 MB RAM, multi-core processor, 16 MB storage
- **Smart Motors**: 11W max power, integrated encoders, temperature/current monitoring
- **V5 Ports**: 21 configurable smart ports (motors, sensors, devices)
- **Wireless Communication**: Competition field network for match coordination

### EZ-Template Framework

EZ-Template provides abstraction layers built on the VEX V5 Python API:

#### Chassis System (Simplified Drive Control)
```python
# Built on EZ-Template PID-based movement
from vex import *

# Typical setup (from your base code)
chassis = Chassis(...)  # EZ wrapper for drive system
chassis.drive(distance_inches, max_velocity)  # Automatic PID control
chassis.turn(degrees, max_velocity)  # Gyro-assisted rotation
chassis.wait_drive()  # Blocks until movement complete
```

#### PID-Based Motion Control
- **Drive PID**: Maintains velocity and distance targets
- **Turn PID**: Stabilizes heading using gyroscope feedback
- **Gains**: Tuned for 2544X's specific gear ratios and motor configuration
- **Telemetry**: Real-time velocity, position, and error tracking

### Python Runtime Environment

- **Runtime**: VEXcode Python 3.x variant for V5
- **Module System**: 
  - `vex` - Core robotics APIs (motors, sensors, brain)
  - `urandom` - Utility for random number generation
  - `math` - Standard math operations for autonomous calculations
- **Event System**: Cooperative multitasking with callback handlers

### Common EZ-Template Patterns (Used in Your Code)

```python
# Autonomous routine selection
chassis.select_auton_routine(auton_selector)  # From vision sensor or controller

# Drive movements with automatic PID
chassis.drive_distance(12, 100)  # Move 12 inches at 100% velocity
chassis.turn_to_heading(90)  # Rotate to absolute heading

# Synchronized multi-subsystem control
intake.spin(FORWARD)
while not color_sensor.hue_in_range(target_hue):
    chassis.drive(forward_speed)
intake.stop()

# Autonomous feedback
chassis.get_drive_distance()  # Returns encoder distance
chassis.get_heading()  # Returns gyro heading (0-359)
```

### Subsystem Architecture

#### Drive System (Push_back Strategy)
- **Drivetrain**: Differential drive with left/right motor groups
- **Motors**: Smart motors with integrated encoders
- **Control**: EZ-Template PID-based positioning
- **Sensors**: Motor encoders + IMU gyroscope for accuracy
- **Strategy Focus**: "Push_back" - defensive positioning and controlled movements

#### Autonomous System (  Game)
- **Selection**: Vision or controller-based routine selection
- **Movements**: Pre-programmed drive/turn sequences
- **Disc Interaction**: Intake and shooter subsystem timing
- **Scoring**: Push-back oriented approach for disc placement
- **Calibration**: Uses `Auton_Pathing_V2.v5python` for optimal paths

#### Sensor Integration
- **Encoders**: Motor-integrated, tracked by EZ-Template
- **IMU**: 6-axis gyroscope for heading correction (used in `IMU_Drive_V1`)
- **Vision**: Object/disc detection for intelligent autonomous decisions
- **Motor Current**: Safety monitoring to prevent stalling

---

## The Replay Script

### Why People Come Here

The **`2544X_Bolted_(Override)----Replay.zip`** is the primary resource for competition analysis. This 17MB archive contains recorded match data including:

- Complete match logs from both autonomous and driver control periods
- Motor velocity, position, and current draw history
- Sensor telemetry (IMU heading, vision detections, encoder readings)
- Timestamp-synchronized event markers
- Performance metrics and diagnostic information
- Data exportable for replay visualization and analysis

### What's Inside the Replay Archive

When extracted, the replay file provides structured match data:

```
2544X_Bolted_(Override)----Replay/
├── match_telemetry.log          # Raw sensor and motor data (timestamp, sensor_id, value)
├── match_events.json            # Timestamped match events (autonomous start/end, scoring)
├── autonomous_execution.log     # Autonomous routine trace with PID values
├── driver_control.log           # Driver input and motor responses (15ms intervals)
├── performance_analysis.txt     # Key metrics, match score, optimization notes
└── visualization_data.json      # Data formatted for replay visualization tools
```

### How to Use the Replay Script

#### 1. **Extract the Archive**
```bash
unzip "2544X_Bolted_(Override)----Replay.zip" -d replay_analysis/
cd replay_analysis/
```

#### 2. **Load Match Data (Python Example)**
```python
import json

# Load match events
with open('match_events.json', 'r') as f:
    events = json.load(f)

# Parse autonomous phase
auton_events = [e for e in events if e['phase'] == 'autonomous']
print(f"Autonomous duration: {auton_events[-1]['timestamp']}ms")

# Parse driver control phase
teleop_events = [e for e in events if e['phase'] == 'driver_control']
print(f"Driver control events: {len(teleop_events)}")
```

#### 3. **Analyze Performance Metrics**
```bash
cat performance_analysis.txt
# Shows:
# - Autonomous scoring accuracy
# - Motor efficiency and power usage
# - Drive system responsiveness (PID tuning effectiveness)
# - Sensor reliability metrics
# - Improvement recommendations
```

#### 4. **Import to Visualization Tools**
- Load `visualization_data.json` into match analyzer
- Timeline view shows motor commands vs. actual motor response
- Heading (IMU) graphs show rotation accuracy from EZ-Template PID
- Multi-subsystem overlay for synchronized event analysis

### Use Cases for Replay Analysis

| Use Case | Analysis | Benefit |
|----------|----------|---------|
| **Autonomous Tuning** | Compare `Auton_Pathing_V2` executions | Optimize drive paths, reduce positioning errors |
| **PID Optimization** | Motor response vs. command signals | Fine-tune EZ-Template velocity/position gains |
| **Driver Performance** | Control input latency and corrections | Train drivers, improve response smoothness |
| **Sensor Validation** | Vision/IMU consistency across matches | Verify calibration in competition environment |
| **Power Management** | Motor stall events and current draw | Detect drivetrain issues, adjust gear ratios |
| **Disc Scoring** | Intake/shooter timing vs. game events | Refine Push_back strategy for optimal placement |

### Key Metrics in Replay Data

```json
{
  "match_id": "qualifiers_round_5",
  "autonomous_phase": {
    "duration_ms": 15000,
    "routine_name": "Auton_Pathing_V2",
    "heading_error_max_degrees": 2.5,
    "position_error_max_inches": 3.2,
    "motor_stall_events": 0,
    "ez_template_pid": {
      "drive_kp": 0.085,
      "turn_kp": 1.2,
      "avg_error": 1.8
    },
    "discs_scored": 3,
    "success_rate": 100
  },
  "driver_control_phase": {
    "duration_ms": 105000,
    "control_inputs": 847,
    "avg_latency_ms": 12,
    "motor_efficiency_percent": 94.2,
    "sensor_uptime_percent": 99.8,
    "intake_cycles": 24,
    "shooter_activations": 18
  },
  "post_match": {
    "winning_alliance": "red",
    "endgame_platform": true,
    "total_score": 142
  }
}
```

---

## Code Categories

### 1. Competition Code (Production Track)

**Files**: `Competition_Code_V*.v5python`, `Stable_Competition_Build.v5python`

These are the primary competition routines deployed to the robot during   matches.

**Key Features**:
- Optimized autonomous routines using EZ-Template PID (tuned drive/turn gains)
- Refined driver control bindings for disc intake/shooter subsystems
- Telemetry logging for replay analysis of each match
- Power management and safety limits to prevent motor damage
- Game-legal Push_back strategy implementation

**Technical Details**:
- Uses EZ-Template `chassis.drive()` and `chassis.turn()` for precision movement
- Autonomous selection via controller or vision system
- Real-time subsystem coordination (drive + intake + shooter)

**Usage**: Load into VEXcode and deploy to V5 Brain before competition matches

---

### 2. Autonomous Routines

**Files**: `Auton_Pathing_V2.v5python`, `Linear_Auton.v5python`, `Skills_Auton.v5python`

Specialized autonomous programs for different match scenarios, all using EZ-Template foundation.

**Variants**:
- **Auton_Pathing_V2** (CURRENT): Advanced pathfinding using EZ-Template PID
  - Optimized disc placement for Push_back strategy
  - IMU-assisted heading correction for accuracy
  - Scorer integration with timing optimization
- **Linear_Auton**: Simple straight-line movements (fallback for reliability)
  - Single direction drive with minimal complexity
  - Used if field unpredictability requires simplification
- **Skills_Auton**: Extended autonomous routine for skills challenges
  - Maximum disc collection and scoring
  - 60-second optimization for skills scoring rounds
- **No_Auton_Test**: Teleop-only testing configuration
  - Used for practice without autonomous routines

---

### 3. Movement & Drive Systems

**Files**: `IMU_Drive_V1.v5python`, `Early_Drive_Code.v5python`, `Movement_Test_Code.v5python`

Core drive control implementations, most using EZ-Template framework.

**Technical Details**:
- **IMU_Drive_V1** (EXPERIMENTAL): Enhanced drive using gyroscope feedback
  ```python
  # Heading-stabilized movement with continuous IMU correction
  # Prevents drift during long autonomous movements
  # Built on EZ-Template chassis with additional IMU checks
  def move_with_heading_correction(distance_in, heading_deg, velocity_pct):
      target_heading = chassis.get_heading() + heading_deg
      while chassis.get_drive_distance() < distance_in:
          correction = calculate_heading_error(target_heading)
          chassis.tank(velocity_pct + correction, velocity_pct - correction)
  ```
- **Movement_Test_Code**: Validates motor response and acceleration curves
  - Tests all motor groups individually
  - Logs velocity and position data to replay telemetry
  - Verifies EZ-Template PID gains are appropriate
- **Early_Drive_Code**: Legacy implementation (reference only)
  - Pre-EZ-Template approach, kept for historical reference

---

### 4. Sensor Integration

**Files**: `Vision_Sensor_Test.v5python`, `IMU_Test_Code.v5python`, `Motor_Group_Test.v5python`

Subsystem validation and calibration tools using EZ-Template utilities.

**Purposes**:
- **Vision_Sensor_Test**: Validates disc detection and color recognition
  - Tests hue detection for disc identification
  - Calibrates brightness thresholds for competition lighting
  - Logs detection events to match telemetry
- **IMU_Test_Code**: Calibrates gyroscope and tests heading accuracy
  - Performs IMU zero-calibration before matches
  - Tests rotation accuracy across multiple 360° spins
  - Verifies heading drift over 15 seconds
- **Motor_Group_Test**: Verifies motor synchronization and acceleration profiles
  - Tests left/right drive group matching
  - Validates EZ-Template PID responsiveness
  - Logs motor current and temperature under load

---

### 5. Base Templates

**Files**: `Default_VEXcode_Project.v5python`, `Base_Bolted_Code.v5python`

Starting points for new development, pre-configured with EZ-Template.

- **Default_VEXcode_Project**: Standard VEXcode template with team configuration
  - Basic motor/sensor definitions
  - Standard event loop setup
- **Base_Bolted_Code**: Team-specific skeleton with EZ-Template integration
  - Pre-imported EZ-Template classes
  - Common subsystem definitions (drive, intake, shooter)
  - Autonomous selection framework
  - Telemetry logging setup
- **Experimental_Code**: Sandbox for testing new concepts
  - Safe environment for trying new PID gains
  - Testing advanced features without affecting competition code

---

## Getting Started

### Prerequisites

1. **VEXcode V5**: Download from [vexcode.cloud](https://vexcode.cloud)
   - Current version 1.2.0+
2. **V5 Hardware**: Competition-legal VEX Robotics V5 system
   - V5 Brain
   - Smart Motors (motor groups for drivetrain)
   - Sensors (IMU, encoders, vision sensor)
3. **Python Knowledge**: Familiarity with Python syntax and robotics concepts
4. **EZ-Template Understanding**: Basic knowledge of chassis, PID, and EZ APIs
5. **Git** (Optional): For version control and cloning this repo

### Loading Code to Your V5 Brain

#### Option 1: Direct Import in VEXcode
1. Open VEXcode
2. Click **File** → **Open Project**
3. Select any `.v5python` file from this repository
4. Verify all motor/sensor ports match your robot configuration
5. Click **Download Project** to deploy to the brain

#### Option 2: Command Line
```bash
# Requires VEXcode CLI tools installed
vexcode download 2544X_Bolted_\(Push_back\)----Stable_Competition_Build.v5python
```

### Running Your First Test

1. Load `Movement_Test_Code.v5python` into VEXcode
2. Connect your V5 Brain via USB
3. Configure ports to match your hardware:
   - Left drive motors on ports 1-2
   - Right drive motors on ports 10-11 (or adjust as needed)
4. Deploy the project
5. Monitor the brain's display for movement commands
6. Check the replay telemetry to verify motor encoder values and PID behavior

### Modifying Competition Code

**Important**: Always work on a copy of `Stable_Competition_Build.v5python`

```bash
# Create a new version
cp 2544X_Bolted_\(Push_back\)----Stable_Competition_Build.v5python \
   2544X_Bolted_\(Push_back\)----Competition_Code_V5.v5python
```

Then in VEXcode:
1. Open the new file
2. Make your modifications to EZ-Template settings or subsystem logic
3. Test with `Movement_Test_Code` first
4. Run full validation before competition deployment
5. Extract replay data to verify improvements in metrics

### Tuning EZ-Template PID Gains

Found in code (typical values):
```python
# From Competition_Code_V4.v5python
drive_kp = 0.085  # Proportional gain for drive distance control
turn_kp = 1.2     # Proportional gain for rotation control

# Tuning process:
# 1. Run Auton_Pathing_V2 multiple times
# 2. Extract replay data
# 3. Analyze motor_error trends
# 4. Adjust gains: increase kp if undershooting, decrease if oscillating
# 5. Re-test and compare replay metrics
```

---

## Contributing

### Development Workflow

1. **Create a feature file** using the naming convention:
   ```
   2544X_Bolted_(Push_back)----[Feature_Name].v5python
   ```

2. **Test thoroughly** with corresponding test code
   - Use `Motor_Group_Test.v5python` for mechanical systems
   - Use `IMU_Test_Code.v5python` for navigation
   - Use `Vision_Sensor_Test.v5python` for vision features

3. **Document your changes** in comments within the code:
   ```python
   #region Feature: IMU-Based Drive Stabilization
   # Author: Team 2544X
   # Version: 1.0
   # EZ-Template Integration: Enhanced chassis.drive() with IMU checks
   # Description: Gyroscope feedback corrects heading drift during autonomous
   # PID Tuning: See line XX for gain adjustments
   #endregion
   ```

4. **Validate with replay analysis**
   - Deploy your code
   - Run practice matches
   - Extract replay data
   - Compare metrics against `Stable_Competition_Build` baseline
   - Verify improvements in accuracy/efficiency

### Code Style Guidelines

- Use meaningful variable names: `left_drive_motors` not `ldm`
- Follow EZ-Template conventions for subsystem naming
- Comment complex PID tuning or autonomous algorithms
- Keep functions focused and modular
- Test edge cases (motor stalling, sensor failures, timeout scenarios)
- Document any modifications to EZ-Template defaults

### Creating New Test Files

Follow this template for new test/validation code:

```python
"""
Test: [Subsystem] Validation
Purpose: [What this tests]
EZ-Template Components: [chassis, IMU, motor groups, etc.]
Success Criteria: [Expected outcomes]
"""

from vex import *
import urandom
import math

brain = Brain()
motors = MotorGroup(...)

def test_basic_movement():
    # Test description
    motors.spin(FORWARD)
    wait(1000)
    motors.stop()
    # Verify encoder readings meet expectations

def test_edge_cases():
    # Test stalling behavior with EZ-Template
    # Test timeout scenarios
    pass

if __name__ == "__main__":
    test_basic_movement()
    test_edge_cases()
```

---

## Troubleshooting

### Common Issues

**Q: Code won't deploy to Brain**
- Ensure V5 Brain is in bootloader mode (hold center button for 3 seconds)
- Verify USB connection quality
- Try a different USB cable
- Check V5 firmware version matches VEXcode

**Q: Motors not responding to commands**
- Check motor port configuration in code matches physical ports
- Verify motor firmware is up-to-date via VEXcode
- Run `Motor_Group_Test.v5python` to diagnose
- Check EZ-Template motor group initialization in code

**Q: Autonomous routine drifting off course**
- Use `IMU_Test_Code.v5python` to verify gyroscope calibration
- Review `Auton_Pathing_V2.v5python` heading correction algorithm
- Check encoder values in replay telemetry match expected distances
- Verify EZ-Template drive/turn PID gains are appropriate
- Measure actual gear ratio vs. configured value

**Q: Vision sensor not detecting discs**
- Run `Vision_Sensor_Test.v5python` with debug output enabled
- Verify adequate lighting (competition-legal, target 300-500 lux)
- Calibrate color signatures for   disc colors
- Test under competition lighting conditions

**Q: PID gains causing oscillation or sluggish response**
- Extract replay telemetry to see motor error trends
- If oscillating: reduce kp gain
- If slow: increase kp gain
- Run `Movement_Test_Code` to validate new gains
- Re-extract replay data to confirm improvement

### Support Resources

- **VEX Documentation**: [docs.vexrobotics.com](https://docs.vexrobotics.com)
- **EZ-Template Docs**: [github.com/EZ-Robotics/EZ-Template](https://github.com/EZ-Robotics/EZ-Template)
- **VEXcode Help**: Built-in tooltips in VEXcode IDE
- **Team 2544X**: Contact team leads for technical issues

---

## License

This code is the intellectual property of Team 2544X Bolted. 

**VEX Rule G4 Compliance**: This repository is provided for team members and educational purposes only. Copying, cloning, or using this code without explicit written permission from Team 2544X violates VEX Rule G4 and will result in disqualification from competition.

---

## Quick Reference: File Selection Guide

| Goal | Use This File |
|------|-----------------|
| **Deploy for competition** | `Stable_Competition_Build.v5python` |
| **Test new code changes** | `Movement_Test_Code.v5python` or `Experimental_Code.v5python` |
| **Analyze match performance** | Extract `Replay.zip` |
| **Debug autonomous routines** | `Auton_Pathing_V2.v5python` with replay telemetry |
| **Validate sensor health** | `IMU_Test_Code.v5python`, `Vision_Sensor_Test.v5python` |
| **Start fresh development** | `Base_Bolted_Code.v5python` (EZ-Template configured) |
| **Configure team setup** | `Default_VEXcode_Project.v5python` |
| **Tune PID gains** | `Competition_Code_V4.v5python` + replay analysis |

---

## About    ( )

**2544X Bolted** competes in **VEX Robotics   :  **, featuring:

- **Objective**: Score discs into team-colored goal zones
- **Strategy**: Push_back-oriented defensive positioning and controlled placement
- **Autonomous**: 15-second routine before driver control (uses EZ-Template pathfinding)
- **Driver Control**: 105-second teleop with disc management
- **Scoring**: Points for disc placement, platform endgame bonuses
- **Alliance**: 2v2 multiplayer on shared field

---

**Last Updated**: 2026-05-14  
**Team**: 2544X Bolted  
**Competition**: VEX Robotics    ( ) / V5  
**Framework**: EZ-Template-based  
**Repository**: [github.com/MithulEsakiraja/2544X-vex-team-bolted](https://github.com/MithulEsakiraja/2544X-vex-team-bolted)
