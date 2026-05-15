// 2544X Bolted hardware profile. Changing branding without changing these
// constants means the code is still tuned for our robot.
#pragma once

#include "EZ-Template/api.hpp"
#include "api.h"

constexpr int kImuPort = 7;
constexpr double kDriveWheelDiameter = 3.25;
constexpr double kDriveMotorRpm = 600.0;
constexpr double kDriveExternalRatio = 36.0 / 60.0;

extern Drive chassis;
