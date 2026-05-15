#pragma once

enum class DriverMode {
  Match,
  Record,
  PitCheck,
};

enum class DriverProfile {
  Full,
  Precision,
  Defense,
  Skills,
};

void driver_assist_initialize();
void driver_assist_match_timer_reset();
void driver_assist_tank_iterate();
bool driver_assist_precision_enabled();
DriverMode driver_assist_mode_get();
DriverProfile driver_assist_profile_get();
int driver_assist_left_output_get();
int driver_assist_right_output_get();
double driver_assist_hottest_drive_temp();
const char* driver_assist_drive_health_label();
const char* driver_assist_mode_name();
const char* driver_assist_profile_name();
