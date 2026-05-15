// 2544X Bolted public replay module.
//
// This public GitHub build keeps the project buildable and uploadable, but the
// working replay recorder is intentionally not included here. The concepts are
// documented in docs/REPLAY_BUILD_GUIDE.md so teams can learn the engineering
// process instead of copying a robot-specific path follower.

#include "path_recorder.hpp"

#include <string>

#include "main.h"

namespace {
PathReplaySlot active_slot = PathReplaySlot::Qual;

const char* kGuideStorageLabel = "GUIDE";

void stop_drive() {
  for (pros::Motor& motor : chassis.left_motors) {
    motor.move(0);
  }
  for (pros::Motor& motor : chassis.right_motors) {
    motor.move(0);
  }
}

void guide_message(const std::string& title, const std::string& detail) {
  master.clear();
  master.print(0, 0, "%s", title.c_str());
  master.print(1, 0, "%s", detail.c_str());
  ez::screen_print(title + "\n" + detail, 1);
}

void replay_guide_notice(PathReplaySlot slot, bool mirrored) {
  stop_drive();
  const std::string title = std::string(mirrored ? "Mirror " : "Replay ") + path_recorder_slot_label(slot);
  guide_message(title, "See docs/replay guide");
}
}  // namespace

void path_recorder_initialize() {}

void path_recorder_iterate() {
  if (master.get_digital_new_press(DIGITAL_R2)) {
    path_recorder_active_slot_set(active_slot == PathReplaySlot::Qual ? PathReplaySlot::Skills : PathReplaySlot::Qual);
    guide_message(std::string("Slot ") + path_recorder_slot_label(active_slot), "Public guide build");
  }

  if (master.get_digital(DIGITAL_L1) &&
      (master.get_digital_new_press(DIGITAL_UP) || master.get_digital_new_press(DIGITAL_DOWN))) {
    guide_message("Recorder removed", "Read replay guide");
  }
}

bool path_recorder_load(std::vector<RecordedPathFrame>& frames) {
  return path_recorder_load(active_slot, frames);
}

bool path_recorder_load(PathReplaySlot, std::vector<RecordedPathFrame>& frames) {
  frames.clear();
  return false;
}

bool path_recorder_saved_path_exists() {
  return path_recorder_saved_path_exists(active_slot);
}

bool path_recorder_saved_path_exists(PathReplaySlot) {
  return false;
}

bool path_recorder_sd_path_exists() {
  return path_recorder_sd_path_exists(active_slot);
}

bool path_recorder_sd_path_exists(PathReplaySlot) {
  return false;
}

bool path_recorder_ram_path_exists() {
  return path_recorder_ram_path_exists(active_slot);
}

bool path_recorder_ram_path_exists(PathReplaySlot) {
  return false;
}

int path_recorder_duration_ms(PathReplaySlot) {
  return 0;
}

int path_recorder_frame_count(PathReplaySlot) {
  return 0;
}

const char* path_recorder_storage_label() {
  return path_recorder_storage_label(active_slot);
}

const char* path_recorder_storage_label(PathReplaySlot) {
  return kGuideStorageLabel;
}

const char* path_recorder_slot_label(PathReplaySlot slot) {
  return slot == PathReplaySlot::Skills ? "SKILLS" : "QUAL";
}

void path_recorder_active_slot_set(PathReplaySlot slot) {
  active_slot = slot;
}

PathReplaySlot path_recorder_active_slot_get() {
  return active_slot;
}

void replay_recorded_path_auton() {
  replay_guide_notice(PathReplaySlot::Qual, false);
}

void replay_skills_path_auton() {
  replay_guide_notice(PathReplaySlot::Skills, false);
}

void replay_mirrored_recorded_path_auton() {
  replay_guide_notice(PathReplaySlot::Qual, true);
}

void replay_mirrored_skills_path_auton() {
  replay_guide_notice(PathReplaySlot::Skills, true);
}
