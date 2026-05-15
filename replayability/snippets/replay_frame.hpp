#pragma once

// Minimal shape of a timed tank-drive replay frame.
// This is a teaching snippet, not the full 2544X private implementation.

struct ReplayFrame {
  int time_ms = 0;
  int left_command = 0;
  int right_command = 0;
  double heading_deg = 0.0;
  double left_position_deg = 0.0;
  double right_position_deg = 0.0;
};

