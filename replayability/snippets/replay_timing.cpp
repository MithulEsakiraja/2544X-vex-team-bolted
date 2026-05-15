// Teaching snippet for timestamp-based replay.
// This is intentionally incomplete. A real robot needs validation,
// correction, brake handling, and fail-safe stops.

void replay_frames(const std::vector<ReplayFrame>& frames) {
  const int start_ms = pros::millis();

  for (const ReplayFrame& frame : frames) {
    while (pros::millis() - start_ms < frame.time_ms) {
      pros::delay(1);
    }

    move_left_drive(frame.left_command);
    move_right_drive(frame.right_command);
  }

  move_left_drive(0);
  move_right_drive(0);
}
