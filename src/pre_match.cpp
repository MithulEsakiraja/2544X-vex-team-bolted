#include "pre_match.hpp"

#include <algorithm>
#include <cstdio>
#include <cstdint>
#include <string>

#include "main.h"

namespace {
constexpr std::uint32_t kBlack = 0x000000;
constexpr std::uint32_t kTan = 0xEADCC5;
constexpr std::uint32_t kDim = 0x24201A;
constexpr std::uint32_t kGood = 0x6CE38D;
constexpr std::uint32_t kWarn = 0xF2C66D;

double hottest_drive_temp() {
  double hottest = 0.0;
  for (const pros::Motor& motor : chassis.left_motors) {
    hottest = std::max(hottest, motor.get_temperature());
  }
  for (const pros::Motor& motor : chassis.right_motors) {
    hottest = std::max(hottest, motor.get_temperature());
  }
  return hottest;
}

void row(int y, const char* label, bool good, const std::string& detail) {
  pros::screen::set_pen(good ? kGood : kWarn);
  pros::screen::fill_circle(24, y - 5, 6);
  pros::screen::set_pen(kTan);
  pros::c::screen_print_at(TEXT_SMALL, 42, y, "%s", label);
  pros::screen::set_pen(good ? kGood : kWarn);
  pros::c::screen_print_at(TEXT_SMALL, 210, y, "%s", detail.c_str());
}
}  // namespace

void pre_match_checklist_show() {
  const double battery_pct = pros::battery::get_capacity();
  const int battery_mv = pros::battery::get_voltage();
  const double hottest_temp = hottest_drive_temp();
  const bool battery_ok = battery_pct >= 35.0 && battery_mv >= 7600;
  const bool temp_ok = hottest_temp < 55.0;
  const bool imu_ok = chassis.drive_imu_calibrated();
  const bool path_ok = path_recorder_saved_path_exists();

  pros::screen::set_eraser(kBlack);
  pros::screen::erase();
  pros::screen::set_pen(kTan);
  pros::screen::fill_rect(0, 0, 480, 8);
  pros::screen::fill_rect(0, 232, 480, 239);
  pros::c::screen_print_at(TEXT_MEDIUM, 18, 28, "%s", "2544X PRE-MATCH CHECK");
  pros::screen::set_pen(kDim);
  pros::screen::fill_rect(18, 58, 462, 60);

  row(82, "IMU calibrated", imu_ok, imu_ok ? "READY" : "WAIT");
  row(108, "Recorded path", path_ok, path_ok ? "FOUND" : "NO SD FILE");
  row(134, "Battery", battery_ok, util::to_string_with_precision(battery_pct, 0) + "%  " + std::to_string(battery_mv) + "mV");
  row(160, "Drive temp", temp_ok, util::to_string_with_precision(hottest_temp, 0) + "C");
  row(186, "Driver profile", true, driver_assist_profile_name());

  pros::screen::set_pen(kTan);
  pros::c::screen_print_at(TEXT_SMALL, 18, 218, "%s", "R1 profile | brain bottom: MATCH / RECORD / PIT | auton: normal or mirrored replay");

  master.clear();
  master.print(0, 0, "%s", imu_ok && battery_ok && temp_ok ? "Checklist OK" : "Check robot");
  master.print(1, 0, "%s", path_ok ? "Path file found" : "No path file");
  master.print(2, 0, "Bat %.0f%% Temp %.0f", battery_pct, hottest_temp);
}
