// 2544X Bolted driver assist layer. Driver modes, profiles, and warnings are
// built around this team's match workflow.
#include "driver_assist.hpp"

#include <algorithm>
#include <cmath>
#include <string>
#include <vector>

#include "main.h"

namespace {
constexpr int kJoystickDeadband = 8;
constexpr double kTipSlowAngle = 10.0;
constexpr double kTipHardAngle = 15.0;
constexpr int kTipSlowSpeed = 78;
constexpr int kTipHardSpeed = 48;
constexpr int kCreepSpeed = 52;
constexpr double kRightDriveScale = 1.08;
constexpr int kRightMinMovingPower = 10;
constexpr double kWarmTempC = 55.0;
constexpr double kHotTempC = 65.0;
constexpr int kDriverPeriodMs = 105000;
constexpr int kModeRedrawMs = 3000;

DriverProfile driver_profile = DriverProfile::Full;
int left_output = 0;
int right_output = 0;
int last_temp_check_ms = 0;
int last_temp_alert_ms = 0;
int match_start_ms = 0;
bool warned_30s = false;
bool warned_15s = false;
bool warned_5s = false;
DriverMode driver_mode = DriverMode::Match;
int last_mode_draw_ms = 0;
int last_controller_status_ms = 0;

int apply_deadband(int input) {
  return std::abs(input) < kJoystickDeadband ? 0 : input;
}

int sign_of(int input) {
  return input > 0 ? 1 : (input < 0 ? -1 : 0);
}

int right_drive_calibrate(int input, int limit) {
  if (input == 0) {
    return 0;
  }

  int output = static_cast<int>(std::round(input * kRightDriveScale));
  if (std::abs(output) < kRightMinMovingPower) {
    output = sign_of(output) * kRightMinMovingPower;
  }
  return std::clamp(output, -limit, limit);
}

int approach(int current, int target, int step) {
  if (target > current) {
    return std::min(current + step, target);
  }
  if (target < current) {
    return std::max(current - step, target);
  }
  return current;
}

int profile_speed_limit() {
  switch (driver_profile) {
    case DriverProfile::Precision:
      return 70;
    case DriverProfile::Defense:
      return 92;
    case DriverProfile::Skills:
      return 104;
    case DriverProfile::Full:
    default:
      return 127;
  }
}

int profile_output_step() {
  if (master.get_digital(DIGITAL_L2) && driver_mode != DriverMode::PitCheck) {
    return 4;
  }

  switch (driver_profile) {
    case DriverProfile::Precision:
      return 6;
    case DriverProfile::Defense:
      return 12;
    case DriverProfile::Skills:
      return 8;
    case DriverProfile::Full:
    default:
      return 10;
  }
}

void move_side(std::vector<pros::Motor>& motors, int output) {
  for (pros::Motor& motor : motors) {
    motor.move(output);
  }
}

void stop_drive() {
  move_side(chassis.left_motors, 0);
  move_side(chassis.right_motors, 0);
  left_output = 0;
  right_output = 0;
}

int current_speed_limit() {
  int limit = profile_speed_limit();
  if (master.get_digital(DIGITAL_L2) && driver_mode != DriverMode::PitCheck) {
    limit = std::min(limit, kCreepSpeed);
  }

  const double lean = std::max(std::fabs(chassis.imu.get_pitch()), std::fabs(chassis.imu.get_roll()));

  if (lean >= kTipHardAngle) {
    limit = std::min(limit, kTipHardSpeed);
  } else if (lean >= kTipSlowAngle) {
    limit = std::min(limit, kTipSlowSpeed);
  }

  return limit;
}

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

const char* drive_health_label() {
  bool sensor_error = false;
  double hottest = 0.0;

  for (const pros::Motor& motor : chassis.left_motors) {
    const double temp = motor.get_temperature();
    if (!std::isfinite(temp) || temp < 0.0) {
      sensor_error = true;
    } else {
      hottest = std::max(hottest, temp);
    }
  }
  for (const pros::Motor& motor : chassis.right_motors) {
    const double temp = motor.get_temperature();
    if (!std::isfinite(temp) || temp < 0.0) {
      sensor_error = true;
    } else {
      hottest = std::max(hottest, temp);
    }
  }

  if (sensor_error) {
    return "CHECK";
  }
  if (hottest >= kHotTempC) {
    return "HOT";
  }
  if (hottest >= kWarmTempC) {
    return "WARM";
  }
  return "OK";
}

void temperature_warning_iterate() {
  const int now = pros::millis();
  if (now - last_temp_check_ms < 1000) {
    return;
  }
  last_temp_check_ms = now;

  const double temp = hottest_drive_temp();
  if (temp < kWarmTempC) {
    return;
  }

  const bool hot = temp >= kHotTempC;
  if (now - last_temp_alert_ms > (hot ? 2500 : 6000)) {
    last_temp_alert_ms = now;
  }

  const std::string message = std::string(hot ? "HOT DRIVE " : "Warm drive ") +
                              util::to_string_with_precision(temp, 0) + "C";
  master.print(2, 0, "%s", message.c_str());
}

DriverProfile next_profile(DriverProfile profile) {
  switch (profile) {
    case DriverProfile::Full:
      return DriverProfile::Precision;
    case DriverProfile::Precision:
      return DriverProfile::Defense;
    case DriverProfile::Defense:
      return DriverProfile::Skills;
    case DriverProfile::Skills:
    default:
      return DriverProfile::Full;
  }
}

void profile_toggle_iterate() {
  if (master.get_digital_new_press(DIGITAL_R1)) {
    driver_profile = next_profile(driver_profile);
    master.print(0, 0, "Profile: %s", driver_assist_profile_name());
  }
}

const char* mode_name_get() {
  switch (driver_mode) {
    case DriverMode::Record:
      return "RECORD";
    case DriverMode::PitCheck:
      return "PIT CHECK";
    case DriverMode::Match:
    default:
      return "MATCH";
  }
}

void draw_mode_button(int x0, int x1, const char* label, bool selected) {
  pros::screen::set_pen(selected ? 0xEADCC5 : 0x24201A);
  pros::screen::fill_rect(x0, 204, x1, 239);
  pros::screen::set_pen(selected ? 0x000000 : 0xEADCC5);
  pros::c::screen_print_at(TEXT_SMALL, x0 + 48, 218, "%s", label);
}

void mode_select_draw(bool force = false) {
  const int now = pros::millis();
  if (!force && now - last_mode_draw_ms < kModeRedrawMs) {
    return;
  }
  last_mode_draw_ms = now;

  pros::screen::set_pen(0x050505);
  pros::screen::fill_rect(0, 200, 479, 239);
  draw_mode_button(0, 158, "MATCH", driver_mode == DriverMode::Match);
  draw_mode_button(160, 318, "REC", driver_mode == DriverMode::Record);
  draw_mode_button(320, 479, "PIT", driver_mode == DriverMode::PitCheck);
}

void mode_select_iterate() {
  if (pros::competition::is_connected()) {
    if (driver_mode != DriverMode::Match) {
      driver_mode = DriverMode::Match;
      master.print(0, 0, "%s", "Mode: MATCH");
      mode_select_draw(true);
    } else {
      mode_select_draw();
    }
    return;
  }

  mode_select_draw();
  const pros::screen_touch_status_s_t touch = pros::screen::touch_status();
  if (touch.touch_status != TOUCH_PRESSED) {
    return;
  }

  DriverMode selected = driver_mode;
  if (touch.y >= 204 && touch.x < 160) {
    selected = DriverMode::Match;
  } else if (touch.y >= 204 && touch.x < 320) {
    selected = DriverMode::Record;
  } else if (touch.y >= 204) {
    selected = DriverMode::PitCheck;
  }

  if (selected != driver_mode) {
    driver_mode = selected;
    mode_select_draw(true);
    master.print(0, 0, "Mode: %s", mode_name_get());
  }
}

void match_timer_iterate() {
  if (!pros::competition::is_connected() || match_start_ms == 0) {
    return;
  }

  const int elapsed = pros::millis() - match_start_ms;
  const int remaining = std::max(0, kDriverPeriodMs - elapsed);

  if (!warned_30s && remaining <= 30000) {
    warned_30s = true;
    master.rumble(".");
    master.print(1, 0, "%s", "30 seconds left");
  }

  if (!warned_15s && remaining <= 15000) {
    warned_15s = true;
    master.rumble(". .");
    master.print(1, 0, "%s", "15 seconds left");
  }

  if (!warned_5s && remaining <= 5000) {
    warned_5s = true;
    master.rumble("---");
    master.print(1, 0, "%s", "5 seconds left");
  }
}

void test_motor(pros::Motor& motor, const char* name) {
  master.clear();
  master.print(0, 0, "%s", name);
  master.print(1, 0, "%s", "Testing motor");
  motor.move(45);
  pros::delay(450);
  motor.move(-35);
  pros::delay(250);
  motor.move(0);
  pros::delay(250);
}

void pit_check_run() {
  if (pros::competition::is_connected()) {
    master.print(0, 0, "%s", "Pit check locked");
    master.print(1, 0, "%s", "Field connected");
    return;
  }

  master.clear();
  master.print(0, 0, "%s", "Pit check start");
  pros::delay(500);

  if (chassis.left_motors.size() > 0) {
    test_motor(chassis.left_motors[0], "Left front");
  }
  if (chassis.left_motors.size() > 1) {
    test_motor(chassis.left_motors[1], "Left back");
  }
  if (chassis.right_motors.size() > 0) {
    test_motor(chassis.right_motors[0], "Right front");
  }
  if (chassis.right_motors.size() > 1) {
    test_motor(chassis.right_motors[1], "Right back");
  }

  stop_drive();
  master.clear();
  master.print(0, 0, "%s", "Pit check done");
  master.print(1, 0, "Max temp %.0fC", hottest_drive_temp());
  master.print(2, 0, "IMU %.0f/%.0f", chassis.imu.get_pitch(), chassis.imu.get_roll());
}

void pit_check_iterate() {
  if (driver_mode != DriverMode::PitCheck) {
    return;
  }

  if (master.get_digital_new_press(DIGITAL_L2)) {
    pit_check_run();
  }
}

void controller_status_iterate() {
  if (!pros::competition::is_connected() || driver_mode != DriverMode::Match || pros::competition::is_autonomous()) {
    return;
  }

  const int now = pros::millis();
  if (now - last_controller_status_ms < 900) {
    return;
  }
  last_controller_status_ms = now;

  master.print(0, 0, "%s %s %.1fs", competition_ui_selected_auton_name(),
               competition_ui_selected_auton_ready() ? "OK" : "NO",
               competition_ui_selected_auton_duration_ms() / 1000.0);
  master.print(1, 0, "Q%.1fs S%.1fs", path_recorder_duration_ms(PathReplaySlot::Qual) / 1000.0,
               path_recorder_duration_ms(PathReplaySlot::Skills) / 1000.0);
  master.print(2, 0, "BAT %.0f DRV %s", pros::battery::get_capacity(), drive_health_label());
}
}  // namespace

void driver_assist_initialize() {
  driver_profile = DriverProfile::Full;
  left_output = 0;
  right_output = 0;
}

void driver_assist_match_timer_reset() {
  match_start_ms = pros::millis();
  warned_30s = false;
  warned_15s = false;
  warned_5s = false;
}

void driver_assist_tank_iterate() {
  mode_select_iterate();
  profile_toggle_iterate();
  temperature_warning_iterate();
  match_timer_iterate();
  pit_check_iterate();
  controller_status_iterate();

  const int limit = current_speed_limit();
  int left_target = apply_deadband(master.get_analog(ANALOG_LEFT_Y));
  int right_target = apply_deadband(master.get_analog(ANALOG_RIGHT_Y));

  left_target = std::clamp(left_target, -limit, limit);
  right_target = std::clamp(right_target, -limit, limit);
  right_target = right_drive_calibrate(right_target, limit);

  const int output_step = profile_output_step();
  left_output = approach(left_output, left_target, output_step);
  right_output = approach(right_output, right_target, output_step);

  move_side(chassis.left_motors, left_output);
  move_side(chassis.right_motors, right_output);
}

bool driver_assist_precision_enabled() {
  return driver_profile == DriverProfile::Precision;
}

DriverMode driver_assist_mode_get() {
  return driver_mode;
}

DriverProfile driver_assist_profile_get() {
  return driver_profile;
}

int driver_assist_left_output_get() {
  return left_output;
}

int driver_assist_right_output_get() {
  return right_output;
}

double driver_assist_hottest_drive_temp() {
  return hottest_drive_temp();
}

const char* driver_assist_drive_health_label() {
  return drive_health_label();
}

const char* driver_assist_mode_name() {
  return mode_name_get();
}

const char* driver_assist_profile_name() {
  switch (driver_profile) {
    case DriverProfile::Precision:
      return "PRECISION";
    case DriverProfile::Defense:
      return "DEFENSE";
    case DriverProfile::Skills:
      return "SKILLS";
    case DriverProfile::Full:
    default:
      return "FULL";
  }
}
