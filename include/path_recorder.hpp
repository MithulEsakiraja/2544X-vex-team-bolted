#pragma once

#include <vector>

struct RecordedPathFrame {
  int time_ms = 0;
  int left = 0;
  int right = 0;
  double heading = 0.0;
  double left_position = 0.0;
  double right_position = 0.0;
};

enum class PathReplaySlot {
  Qual,
  Skills,
};

void path_recorder_initialize();
void path_recorder_iterate();
bool path_recorder_load(std::vector<RecordedPathFrame>& frames);
bool path_recorder_load(PathReplaySlot slot, std::vector<RecordedPathFrame>& frames);
bool path_recorder_saved_path_exists();
bool path_recorder_saved_path_exists(PathReplaySlot slot);
bool path_recorder_sd_path_exists();
bool path_recorder_sd_path_exists(PathReplaySlot slot);
bool path_recorder_ram_path_exists();
bool path_recorder_ram_path_exists(PathReplaySlot slot);
int path_recorder_duration_ms(PathReplaySlot slot);
int path_recorder_frame_count(PathReplaySlot slot);
const char* path_recorder_storage_label();
const char* path_recorder_storage_label(PathReplaySlot slot);
const char* path_recorder_slot_label(PathReplaySlot slot);
void path_recorder_active_slot_set(PathReplaySlot slot);
PathReplaySlot path_recorder_active_slot_get();
void replay_recorded_path_auton();
void replay_skills_path_auton();
void replay_mirrored_recorded_path_auton();
void replay_mirrored_skills_path_auton();
