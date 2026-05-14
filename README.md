# 2544X Bolted - VEX Robotics Team Repository

Welcome to the **2544X Bolted** VEX Robotics team repository! This is the official codebase for Team 2544X's VEX platform competition code and experimental systems.

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
- **`2544X_Bolted_(Push_back)----Stable_Competition_Build.v5python`** - Latest stable version
- **`2544X_Bolted_(Push_back)----Competition_Code_V4.v5python`** - Current active code

---

## Repository Overview

This repository contains the complete software suite for the 2544X Bolted VEX team, including:

- **Competition Code**: Production-ready autonomous and driver control routines
- **Test & Validation Scripts**: Subsystem verification and calibration code
- **Experimental Systems**: Advanced features under development (IMU-based movement, vision processing)
- **Match Replays**: Recorded competition data for performance analysis
- **Legacy Versions**: Historical code for reference and regression testing

### Key Technical Stack

- **Language**: Python (VEXcode Python)
- **Platform**: VEX Robotics Generation 4 (V5)
- **API**: VEX V5 Python SDK
- **Version Control**: Git (with VEXcode project files)

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
| **STRATEGY** | `Push_back`, `Override` | `Push_back`: Primary strategy variant<br>`Override`: Special purpose (replays, override behavior) |
| **PURPOSE** | Descriptive name | Clearly indicates function (e.g., `Competition_Code`, `IMU_Test_Code`) |
| **Extension** | `.v5python` | VEXcode Python project file format |

### Organization Structure

```
2544X-vex-team-bolted/
├── Competition Code
│   ├── 2544X_Bolted_(Push_back)----Stable_Competition_Build.v5python     [PRODUCTION]
│   ├── 2544X_Bolted_(Push_back)----Competition_Code_V4.v5python          [ACTIVE]
│   ├── 2544X_Bolted_(Push_back)----Competition_Code_V3.v5python          [PREVIOUS]
│   └── 2544X_Bolted_(Push_back)----Skills_Auton.v5python                 [SKILLS EVENT]
│
├── Autonomous Routines
│   ├── 2544X_Bolted_(Push_back)----Auton_Pathing_V2.v5python             [CURRENT]
│   ├── 2544X_Bolted_(Push_back)----Linear_Auton.v5python                 [BASIC]
│   └── 2544X_Bolted_(Push_back)----No_Auton_Test.v5python                [TEST]
│
├── Movement & Drive Systems
│   ├── 2544X_Bolted_(Push_back)----Early_Drive_Code.v5python             [LEGACY]
│   ├── 2544X_Bolted_(Push_back)----IMU_Drive_V1.v5python                 [EXPERIMENTAL]
│   └── 2544X_Bolted_(Push_back)----Movement_Test_Code.v5python           [VALIDATION]
│
├── Sensor Integration
│   ├── 2544X_Bolted_(Push_back)----Vision_Sensor_Test.v5python           [VISION]
│   ├── 2544X_Bolted_(Push_back)----IMU_Test_Code.v5python                [IMU CALIBRATION]
│   └── 2544X_Bolted_(Push_back)----Motor_Group_Test.v5python             [MOTOR TEST]
│
├── Base Templates
│   ├── 2544X_Bolted_(Push_back)----Default_VEXcode_Project.v5python      [TEMPLATE]
│   ├── 2544X_Bolted_(Push_back)----Base_Bolted_Code.v5python             [STARTER]
│   └── 2544X_Bolted_(Push_back)----Experimental_Code.v5python            [R&D]
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
| **PRODUCTION** | `Stable_Competition_Build` | Battle-tested, zero known critical issues |
| **ACTIVE** | `Competition_Code_V4` | Currently deployed to robots |
| **CURRENT** | Latest versioned files | Most recent iteration with improvements |
| **EXPERIMENTAL** | `IMU_Drive`, `Experimental_Code` | Under development, use for testing only |
| **LEGACY** | `Early_Drive`, old versions | Superseded, kept for reference/rollback |
| **TEST/VALIDATION** | Test files | Diagnostic and calibration purposes |

---

## Technical Documentation

### VEX V5 Architecture

The codebase is built on the **VEX Robotics V5 System**, featuring:

- **Brain**: Cortex-based main processor with Python runtime
- **Motors**: Smart motors with integrated encoders and temperature monitoring
- **Sensors**: IMU (Inertial Measurement Unit), Vision Sensor, Motor Encoders
- **Wireless**: Competition field network communication

### Python Runtime Environment

- **Runtime**: VEXcode Python (custom Python 3.x variant for V5)
- **Framework**: Event-driven robotics framework
- **Module System**: VEX-specific libraries (brain, motor, sensor APIs)

### Common API Patterns

```python
# Motor Control
motor.spin(FORWARD, speed)  # speed: 0-100%
motor.rotate(degrees, velocity)
motor.set_max_torque(torque)

# Sensor Reading
imu.heading()  # 0-359 degrees
vision_sensor.take_snapshot()
motor.velocity()  # RPM

# Autonomous Routines
# Timed movements
motor.spin(FORWARD)
wait(time_ms)
motor.stop()

# Encoder-based movements
motor.rotate(360)  # 1 complete rotation
```

### Subsystem Architecture

#### Drive System
- **Type**: Differential drive (tank-style)
- **Motors**: Primary drive motors grouped via motor groups
- **Control**: Tank drive (separate left/right input) or arcade drive
- **Sensors**: Encoders on main motors; IMU for gyro stabilization

#### Autonomous Movement
- **Position Control**: Motor encoder-based positioning
- **Heading Control**: IMU gyroscope for absolute angle tracking
- **Pathing**: Linear movements followed by rotations (Push-back strategy)

#### Sensor Integration
- **Vision**: Object detection and line following
- **IMU**: Heading correction and tilt detection
- **Motor Feedback**: Real-time velocity and position monitoring

---

## The Replay Script

### Why People Come Here

The **`2544X_Bolted_(Override)----Replay.zip`** is the primary resource for competition analysis. This file contains recorded match data including:

- Complete match logs (autonomous + driver control periods)
- Motor velocity/position history
- Sensor telemetry (IMU, vision, encoders)
- Timestamp-synchronized event markers
- Performance metrics and diagnostics

### What's Inside the Replay Archive

When extracted, the replay file provides:

```
2544X_Bolted_(Override)----Replay/
├── match_telemetry.log          # Raw sensor and motor data
├── match_events.json            # Timestamped match events
├── autonomous_execution.log     # Auton routine trace
├── driver_control.log           # Driver input and motor responses
├── performance_analysis.txt     # Key metrics and analysis
└── visualization_data.json      # Data for replay visualization tools
```

### How to Use the Replay Script

#### 1. **Extract the Archive**
```bash
unzip 2544X_Bolted_\(Override\)----Replay.zip -d replay_analysis/
```

#### 2. **Load Match Data**
Use VEXcode's replay viewer or custom analysis scripts:
```python
import json

with open('match_events.json', 'r') as f:
    events = json.load(f)

for event in events:
    print(f"Time: {event['timestamp']}, Event: {event['type']}")
```

#### 3. **Analyze Performance Metrics**
```bash
cat performance_analysis.txt
# Shows:
# - Autonomous routine accuracy
# - Motor efficiency during match
# - Sensor reliability
# - Power consumption
```

#### 4. **Replay Visualizations**
- Timeline view of motor positions and velocities
- Heading (IMU) graphs for rotation accuracy
- Sensor detection events overlaid on match timeline
- Comparison with previous match runs

### Use Cases for Replay Analysis

| Use Case | Analysis | Benefit |
|----------|----------|---------|
| **Autonomous Tuning** | Compare auton execution paths | Identify positioning errors, optimize routines |
| **Driver Performance** | Motor response during teleop | Improve control smoothness, reduce overcorrection |
| **Sensor Validation** | Vision/IMU data consistency | Verify sensor calibration and reliability |
| **Power Management** | Motor current draw patterns | Detect stalling, optimize acceleration profiles |
| **Performance Regression** | Compare against baseline runs | Confirm improvements from code updates |

### Key Metrics in Replay Data

```json
{
  "autonomous_phase": {
    "duration_ms": 15000,
    "routine_name": "Auton_Pathing_V2",
    "heading_error_max": 2.5,
    "position_error_max": 3.2,
    "motor_stall_events": 0,
    "success_rate": 100
  },
  "driver_control_phase": {
    "duration_ms": 105000,
    "control_inputs": 847,
    "avg_latency_ms": 12,
    "motor_efficiency": 94.2,
    "sensor_uptime": 99.8
  }
}
```

---

## Code Categories

### 1. Competition Code (Production Track)

**Files**: `Competition_Code_V*.v5python`, `Stable_Competition_Build.v5python`

These are the primary competition routines deployed to the robot during matches.

**Key Features**:
- Optimized autonomous routines for current game rules
- Refined driver control bindings
- Telemetry logging for replay analysis
- Power management and safety limits

**Usage**: Load into VEXcode and deploy to V5 Brain before competition

---

### 2. Autonomous Routines

**Files**: `Auton_Pathing_V*.v5python`, `Linear_Auton.v5python`, `Skills_Auton.v5python`

Specialized autonomous programs for different match types.

**Variants**:
- **Auton_Pathing_V2**: Advanced pathfinding with IMU stabilization
- **Linear_Auton**: Simple straight-line movements (fallback)
- **Skills_Auton**: Extended autonomous for skills challenges
- **No_Auton_Test**: Teleop-only testing configuration

---

### 3. Movement & Drive Systems

**Files**: `IMU_Drive_V1.v5python`, `Early_Drive_Code.v5python`, `Movement_Test_Code.v5python`

Core drive control implementations.

**Technical Details**:
- **IMU_Drive_V1**: Heading-stabilized drive using gyroscope feedback
  ```python
  def move_with_heading(distance, target_heading):
      # Encoder-based movement with continuous IMU correction
      # Prevents drift during long autonomous movements
  ```
- **Movement_Test_Code**: Validates motor response and acceleration curves
- **Early_Drive_Code**: Legacy implementation (reference only)

---

### 4. Sensor Integration

**Files**: `Vision_Sensor_Test.v5python`, `IMU_Test_Code.v5python`, `Motor_Group_Test.v5python`

Subsystem validation and calibration tools.

**Purposes**:
- **Vision_Sensor_Test**: Validates object detection and color recognition
- **IMU_Test_Code**: Calibrates gyroscope and tests heading accuracy
- **Motor_Group_Test**: Verifies motor synchronization and acceleration profiles

---

### 5. Base Templates

**Files**: `Default_VEXcode_Project.v5python`, `Base_Bolted_Code.v5python`

Starting points for new development.

- **Default_VEXcode_Project**: Standard VEXcode template with team configuration
- **Base_Bolted_Code**: Team-specific skeleton with common functions
- **Experimental_Code**: Sandbox for testing new concepts

---

## Getting Started

### Prerequisites

1. **VEXcode V5**: Download from [vexcode.cloud](https://vexcode.cloud)
2. **V5 Brain**: Competition-legal VEX Robotics V5 system
3. **Python Knowledge**: Familiarity with Python syntax and robotics concepts
4. **Git** (Optional): For version control and cloning this repo

### Loading Code to Your V5 Brain

#### Option 1: Direct Import in VEXcode
1. Open VEXcode
2. Click **File** → **Open Project**
3. Select any `.v5python` file from this repository
4. Click **Download Project** to deploy to the brain

#### Option 2: Command Line
```bash
# Requires VEXcode CLI tools installed
vexcode download 2544X_Bolted_\(Push_back\)----Competition_Code_V4.v5python
```

### Running Your First Test

1. Load `Movement_Test_Code.v5python` into VEXcode
2. Connect your V5 Brain via USB
3. Deploy the project
4. Monitor the brain's display for movement commands
5. Check the replay telemetry to verify motor encoder values

### Modifying Competition Code

**Important**: Always work on a copy of `Stable_Competition_Build.v5python`

```bash
# Create a new version
cp 2544X_Bolted_\(Push_back\)----Stable_Competition_Build.v5python \
   2544X_Bolted_\(Push_back\)----Competition_Code_V5.v5python
```

Then in VEXcode:
1. Open the new file
2. Make your modifications
3. Test with `Movement_Test_Code` first
4. Run full validation before competition deployment

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
   # Feature: IMU-Based Drive Stabilization
   # Author: Team 2544X
   # Version: 1.0
   # Description: Gyroscope feedback corrects heading drift during autonomous
   ```

4. **Validate with replay analysis**
   - Deploy your code
   - Run practice matches
   - Extract replay data
   - Compare metrics against baseline

### Code Style Guidelines

- Use meaningful variable names: `left_drive_motors` not `ldm`
- Comment complex algorithms, especially autonomous routines
- Keep functions focused and modular
- Test edge cases (stalling, sensor failures, timeout scenarios)

### Creating New Test Files

Follow this template for new test/validation code:

```python
"""
Test: [Subsystem] Validation
Purpose: [What this tests]
Success Criteria: [Expected outcomes]
"""

brain = Brain()
motors = MotorGroup(...)

def test_basic_movement():
    # Test description
    motors.spin(FORWARD)
    wait(1000)
    motors.stop()
    # Verify encoder readings meet expectations

def test_edge_cases():
    # Test stalling behavior
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

**Q: Motors not responding to commands**
- Check motor configuration in code matches physical port assignments
- Verify motor firmware is up-to-date via VEXcode
- Run `Motor_Group_Test.v5python` to diagnose

**Q: Autonomous routine drifting off course**
- Use `IMU_Test_Code.v5python` to verify gyroscope calibration
- Review `Auton_Pathing_V2.v5python` heading correction algorithm
- Check encoder values match in replay telemetry

**Q: Vision sensor not detecting objects**
- Run `Vision_Sensor_Test.v5python` with debug output enabled
- Verify adequate lighting (competition-legal, under 500 lux)
- Calibrate color signatures for your environment

### Support Resources

- **VEX Documentation**: [docs.vexrobotics.com](https://docs.vexrobotics.com)
- **VEXcode Help**: Built-in tooltips in VEXcode IDE
- **Team 2544X**: Contact team leads for technical issues

---

## License

This code is the intellectual property of Team 2544X Bolted. Use for competition purposes under VEX Robotics competition rules.

---

## Quick Reference: File Selection Guide

| Goal | Use This File |
|------|-----------------|
| **Deploy for competition** | `Stable_Competition_Build.v5python` |
| **Test new code changes** | `Movement_Test_Code.v5python` or `Experimental_Code.v5python` |
| **Analyze match performance** | Extract `Replay.zip` |
| **Debug autonomous routines** | `Auton_Pathing_V2.v5python` with replay telemetry |
| **Validate sensor health** | `IMU_Test_Code.v5python`, `Vision_Sensor_Test.v5python` |
| **Start fresh development** | `Base_Bolted_Code.v5python` |
| **Configure team setup** | `Default_VEXcode_Project.v5python` |

---

**Last Updated**: 2026-05-14  
**Team**: 2544X Bolted  
**Competition**: VEX Robotics G4  
**Repository**: [github.com/MithulEsakiraja/2544X-vex-team-bolted](https://github.com/MithulEsakiraja/2544X-vex-team-bolted)
