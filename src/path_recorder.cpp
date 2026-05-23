// 2544X Bolted replay system. The constants in this file are robot-specific
// calibration values, not universal template settings.
#include "path_recorder.hpp"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdint>
#include <string>

#include "main.h"

namespace {
constexpr const char* kQualPathFile = "/usd/qual_replay.csv";
constexpr const char* kSkillsPathFile = "/usd/skills_replay.csv";
constexpr const char* kQualMetadataFile = "/usd/qual_replay.meta";
constexpr const char* kSkillsMetadataFile = "/usd/skills_replay.meta";
constexpr std::uint32_t kMetadataMagic = 0x42544C44;
constexpr std::uint16_t kReplayFileVersion = 4;
constexpr int kRecordIntervalMs = 1;
constexpr int kRecordingStatusFrameInterval = 500;
constexpr int kQualMaxReplayMs = 15000;
constexpr int kSkillsMaxReplayMs = 60000;
constexpr std::size_t kMinReplayFrames = 10;
constexpr int kReplaySlewStep = 127;
constexpr int kMaxTurnCorrection = 22;
constexpr int kMaxDistanceCorrection = 18;
constexpr int kMaxLeanSlowdown = 72;
constexpr double kRightReplayScale = 1.0;
constexpr int kRightReplayMinMovingPower = 0;
constexpr int kStillCommandDeadband = 26;
constexpr int kReplayMovingCommand = kStillCommandDeadband;
constexpr double kHeadingKp = 0.72;
constexpr double kHeadingDeadbandDeg = 1.2;
constexpr double kDistanceKp = 0.035;
constexpr double kDistanceDeadbandDeg = 12.0;
constexpr double kTipSlowAngle = 9.0;
constexpr double kTipHardAngle = 13.0;
constexpr double kPositionDataThreshold = 1.0;

enum class LoadStatus {
  Missing,
  Valid,
  TooShort,
  OldFormat,
};

struct PathMetadata {
  bool checked = false;
  LoadStatus status = LoadStatus::Missing;
  int duration_ms = 0;
  int frame_count = 0;
};

struct MetadataRecord {
  std::uint32_t magic = 0;
  std::uint16_t version = 0;
  std::uint16_t slot = 0;
  std::int32_t frame_count = 0;
  std::int32_t duration_ms = 0;
  std::uint32_t checksum = 0;
};

std::vector<RecordedPathFrame> qual_frames;
std::vector<RecordedPathFrame> skills_frames;
PathMetadata qual_sd_metadata;
PathMetadata skills_sd_metadata;
PathReplaySlot active_slot = PathReplaySlot::Qual;
bool recording = false;
int record_start_ms = 0;
int last_sample_time = 0;

const char* path_file_get(PathReplaySlot slot) {
  return slot == PathReplaySlot::Skills ? kSkillsPathFile : kQualPathFile;
}

const char* metadata_file_get(PathReplaySlot slot) {
  return slot == PathReplaySlot::Skills ? kSkillsMetadataFile : kQualMetadataFile;
}

std::uint16_t slot_id_get(PathReplaySlot slot) {
  return slot == PathReplaySlot::Skills ? 2 : 1;
}

std::vector<RecordedPathFrame>& frames_for_slot(PathReplaySlot slot) {
  return slot == PathReplaySlot::Skills ? skills_frames : qual_frames;
}

PathMetadata& metadata_for_slot(PathReplaySlot slot) {
  return slot == PathReplaySlot::Skills ? skills_sd_metadata : qual_sd_metadata;
}

void cache_metadata(PathReplaySlot slot, LoadStatus status, const std::vector<RecordedPathFrame>& frames) {
  PathMetadata& metadata = metadata_for_slot(slot);
  metadata.checked = true;
  metadata.status = status;
  metadata.frame_count = static_cast<int>(frames.size());
  metadata.duration_ms = frames.empty() ? 0 : frames.back().time_ms;
}

void cache_metadata(PathReplaySlot slot, LoadStatus status, int frame_count, int duration_ms) {
  PathMetadata& metadata = metadata_for_slot(slot);
  metadata.checked = true;
  metadata.status = status;
  metadata.frame_count = frame_count;
  metadata.duration_ms = duration_ms;
}

void reset_metadata(PathReplaySlot slot) {
  metadata_for_slot(slot) = PathMetadata{};
}

int max_replay_ms(PathReplaySlot slot) {
  return slot == PathReplaySlot::Skills ? kSkillsMaxReplayMs : kQualMaxReplayMs;
}

std::size_t max_replay_frames(PathReplaySlot slot) {
  return static_cast<std::size_t>(max_replay_ms(slot) / kRecordIntervalMs + 2);
}

std::uint32_t metadata_checksum(std::uint16_t slot, int frame_count, int duration_ms) {
  return kMetadataMagic ^ (static_cast<std::uint32_t>(kReplayFileVersion) << 16) ^
         (static_cast<std::uint32_t>(slot) << 8) ^
         (static_cast<std::uint32_t>(frame_count) * 2654435761u) ^
         (static_cast<std::uint32_t>(duration_ms) * 2246822519u);
}

bool metadata_record_valid(PathReplaySlot slot, const MetadataRecord& record) {
  if (record.magic != kMetadataMagic || record.version != kReplayFileVersion || record.slot != slot_id_get(slot)) {
    return false;
  }
  if (record.frame_count < static_cast<std::int32_t>(kMinReplayFrames) || record.duration_ms < 0 ||
      record.duration_ms > max_replay_ms(slot)) {
    return false;
  }
  return record.checksum == metadata_checksum(record.slot, record.frame_count, record.duration_ms);
}

bool save_metadata_file(PathReplaySlot slot, int frame_count, int duration_ms) {
  FILE* file = std::fopen(metadata_file_get(slot), "wb");
  if (file == nullptr) {
    return false;
  }

  MetadataRecord record;
  record.magic = kMetadataMagic;
  record.version = kReplayFileVersion;
  record.slot = slot_id_get(slot);
  record.frame_count = frame_count;
  record.duration_ms = duration_ms;
  record.checksum = metadata_checksum(record.slot, record.frame_count, record.duration_ms);

  const bool wrote = std::fwrite(&record, sizeof(record), 1, file) == 1;
  const bool closed = std::fclose(file) == 0;
  return wrote && closed;
}

LoadStatus load_metadata_file(PathReplaySlot slot) {
  FILE* file = std::fopen(metadata_file_get(slot), "rb");
  if (file == nullptr) {
    cache_metadata(slot, LoadStatus::Missing, 0, 0);
    return LoadStatus::Missing;
  }

  MetadataRecord record;
  const bool read = std::fread(&record, sizeof(record), 1, file) == 1;
  std::fclose(file);

  if (!read || !metadata_record_valid(slot, record)) {
    cache_metadata(slot, LoadStatus::TooShort, 0, 0);
    return LoadStatus::TooShort;
  }

  cache_metadata(slot, LoadStatus::Valid, record.frame_count, record.duration_ms);
  return LoadStatus::Valid;
}

double wrapped_angle_error(double target, double current) {
  double error = target - current;
  while (error > 180.0) {
    error -= 360.0;
  }
  while (error <= -180.0) {
    error += 360.0;
  }
  return error;
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

int scrub_still_command(int input) {
  return std::abs(input) < kStillCommandDeadband ? 0 : input;
}

void move_side(std::vector<pros::Motor>& motors, int output) {
  for (pros::Motor& motor : motors) {
    motor.move(output);
  }
}

void stop_drive() {
  move_side(chassis.left_motors, 0);
  move_side(chassis.right_motors, 0);
}

double motor_position_average(const std::vector<pros::Motor>& motors) {
  if (motors.empty()) {
    return 0.0;
  }

  double sum = 0.0;
  for (const pros::Motor& motor : motors) {
    sum += motor.get_position();
  }
  return sum / motors.size();
}

double left_drive_position() {
  return motor_position_average(chassis.left_motors);
}

double right_drive_position() {
  return motor_position_average(chassis.right_motors);
}

void status_print(const std::string& line1, const std::string& line2 = "") {
  master.clear();
  master.print(0, 0, "%s", line1.c_str());
  master.print(1, 0, "%s", line2.c_str());
  ez::screen_print(line1 + "\n" + line2, 1);
}

void push_frame(int time_ms, int left, int right, double heading, double left_position, double right_position) {
  RecordedPathFrame frame;
  frame.time_ms = std::clamp(time_ms, 0, max_replay_ms(active_slot));
  frame.left = scrub_still_command(std::clamp(left, -127, 127));
  frame.right = scrub_still_command(std::clamp(right, -127, 127));
  frame.heading = heading;
  frame.left_position = left_position;
  frame.right_position = right_position;
  frames_for_slot(active_slot).push_back(frame);
}

bool save_path() {
  FILE* file = std::fopen(path_file_get(active_slot), "w");
  if (file == nullptr) {
    reset_metadata(active_slot);
    status_print("Saved in RAM", std::string(path_recorder_slot_label(active_slot)) + " slot");
    return true;
  }

  bool write_ok = true;
  write_ok = write_ok && std::fprintf(file, "# timed tank replay v4 with heading and distance correction\n") > 0;
  write_ok = write_ok && std::fprintf(file, "# time_ms,left,right,heading,left_position,right_position\n") > 0;
  for (const RecordedPathFrame& frame : frames_for_slot(active_slot)) {
    write_ok = write_ok && std::fprintf(file, "%d,%d,%d,%.3f,%.3f,%.3f\n", frame.time_ms, frame.left, frame.right,
                                        frame.heading, frame.left_position, frame.right_position) > 0;
  }

  write_ok = std::fclose(file) == 0 && write_ok;
  if (!write_ok) {
    reset_metadata(active_slot);
    status_print("SD save failed", "Using RAM copy");
    return true;
  }

  cache_metadata(active_slot, LoadStatus::Valid, frames_for_slot(active_slot));
  const bool metadata_saved = save_metadata_file(active_slot, path_recorder_frame_count(active_slot),
                                                 path_recorder_duration_ms(active_slot));
  status_print(std::string(path_recorder_slot_label(active_slot)) + " saved",
               std::string(metadata_saved ? "SD indexed " : "SD no index ") +
                   std::to_string(frames_for_slot(active_slot).size()) + " frames");
  return true;
}

void start_recording() {
  std::vector<RecordedPathFrame>& frames = frames_for_slot(active_slot);
  frames.clear();
  reset_metadata(active_slot);
  frames.reserve(active_slot == PathReplaySlot::Skills ? 61000 : 16000);
  chassis.pid_targets_reset();
  chassis.drive_sensor_reset();
  chassis.odom_xyt_set(0_in, 0_in, 0_deg);

  record_start_ms = pros::millis();
  last_sample_time = record_start_ms - kRecordIntervalMs;
  recording = true;
  status_print(std::string("Recording ") + path_recorder_slot_label(active_slot), "L1+DOWN saves");
}

void stop_recording() {
  if (!recording) {
    return;
  }

  push_frame(pros::millis() - record_start_ms, 0, 0, chassis.odom_theta_get(), left_drive_position(), right_drive_position());
  recording = false;
  stop_drive();
  save_path();
}

void sample_path() {
  const int now = pros::millis();
  if (now - last_sample_time < kRecordIntervalMs) {
    return;
  }

  const int elapsed = now - record_start_ms;
  if (elapsed > max_replay_ms(active_slot)) {
    stop_recording();
    status_print("Replay capped", active_slot == PathReplaySlot::Skills ? "60 sec skills max" : "15 sec match max");
    return;
  }

  push_frame(elapsed, driver_assist_left_output_get(), driver_assist_right_output_get(), chassis.odom_theta_get(),
             left_drive_position(), right_drive_position());
  last_sample_time = now;

  if (frames_for_slot(active_slot).size() % kRecordingStatusFrameInterval == 0) {
    status_print(std::string("Recording ") + path_recorder_slot_label(active_slot),
                 std::to_string(frames_for_slot(active_slot).size()) + " frames");
  }
}

LoadStatus load_from_file(PathReplaySlot slot, std::vector<RecordedPathFrame>& frames) {
  frames.clear();
  FILE* file = std::fopen(path_file_get(slot), "r");
  if (file == nullptr) {
    cache_metadata(slot, LoadStatus::Missing, frames);
    return LoadStatus::Missing;
  }

  frames.reserve(std::min<std::size_t>(max_replay_frames(slot), slot == PathReplaySlot::Skills ? 61000 : 16000));
  char line[96];
  bool old_format_seen = false;
  while (std::fgets(line, sizeof(line), file) != nullptr) {
    if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') {
      continue;
    }

    RecordedPathFrame frame;
    const int parsed = std::sscanf(line, "%d,%d,%d,%lf,%lf,%lf", &frame.time_ms, &frame.left, &frame.right,
                                   &frame.heading, &frame.left_position, &frame.right_position);
    if (parsed == 3 || parsed == 4) {
      old_format_seen = true;
      continue;
    }
    if (parsed != 6) {
      continue;
    }

    frame.time_ms = std::clamp(frame.time_ms, 0, max_replay_ms(slot));
    if (!frames.empty() && frame.time_ms <= frames.back().time_ms) {
      frame.time_ms = std::min(frames.back().time_ms + 1, max_replay_ms(slot));
    }
    frame.left = scrub_still_command(std::clamp(frame.left, -127, 127));
    frame.right = scrub_still_command(std::clamp(frame.right, -127, 127));
    frames.push_back(frame);

    if (frames.size() >= max_replay_frames(slot) || frame.time_ms >= max_replay_ms(slot)) {
      break;
    }
  }

  std::fclose(file);
  if (frames.size() >= kMinReplayFrames) {
    cache_metadata(slot, LoadStatus::Valid, frames);
    save_metadata_file(slot, static_cast<int>(frames.size()), frames.back().time_ms);
    return LoadStatus::Valid;
  }

  frames.clear();
  const LoadStatus status = old_format_seen ? LoadStatus::OldFormat : LoadStatus::TooShort;
  cache_metadata(slot, status, frames);
  return status;
}

LoadStatus scan_file_metadata(PathReplaySlot slot) {
  FILE* file = std::fopen(path_file_get(slot), "r");
  if (file == nullptr) {
    cache_metadata(slot, LoadStatus::Missing, 0, 0);
    return LoadStatus::Missing;
  }

  char line[96];
  bool old_format_seen = false;
  int frame_count = 0;
  int last_time_ms = 0;

  while (std::fgets(line, sizeof(line), file) != nullptr) {
    if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') {
      continue;
    }

    RecordedPathFrame frame;
    const int parsed = std::sscanf(line, "%d,%d,%d,%lf,%lf,%lf", &frame.time_ms, &frame.left, &frame.right,
                                   &frame.heading, &frame.left_position, &frame.right_position);
    if (parsed == 3 || parsed == 4) {
      old_format_seen = true;
      continue;
    }
    if (parsed != 6) {
      continue;
    }

    frame.time_ms = std::clamp(frame.time_ms, 0, max_replay_ms(slot));
    if (frame_count > 0 && frame.time_ms <= last_time_ms) {
      frame.time_ms = std::min(last_time_ms + 1, max_replay_ms(slot));
    }

    last_time_ms = frame.time_ms;
    ++frame_count;

    if (static_cast<std::size_t>(frame_count) >= max_replay_frames(slot) || frame.time_ms >= max_replay_ms(slot)) {
      break;
    }
  }

  std::fclose(file);

  if (frame_count >= static_cast<int>(kMinReplayFrames)) {
    cache_metadata(slot, LoadStatus::Valid, frame_count, last_time_ms);
    return LoadStatus::Valid;
  }

  const LoadStatus status = old_format_seen ? LoadStatus::OldFormat : LoadStatus::TooShort;
  cache_metadata(slot, status, 0, 0);
  return status;
}

const PathMetadata& sd_metadata_get(PathReplaySlot slot) {
  return metadata_for_slot(slot);
}

bool ram_path_valid(PathReplaySlot slot) {
  return frames_for_slot(slot).size() >= kMinReplayFrames;
}

bool frames_have_position_data(const std::vector<RecordedPathFrame>& frames) {
  for (std::size_t i = 1; i < frames.size(); ++i) {
    const double left_delta = frames[i].left_position - frames[i - 1].left_position;
    const double right_delta = frames[i].right_position - frames[i - 1].right_position;
    if (std::fabs(left_delta) > kPositionDataThreshold || std::fabs(right_delta) > kPositionDataThreshold) {
      return true;
    }
  }
  return false;
}
}  // namespace

void path_recorder_initialize() {
  qual_frames.reserve(900);
  skills_frames.reserve(900);
  load_metadata_file(PathReplaySlot::Qual);
  load_metadata_file(PathReplaySlot::Skills);
}

void path_recorder_iterate() {
  if (!recording && master.get_digital_new_press(DIGITAL_R2)) {
    path_recorder_active_slot_set(active_slot == PathReplaySlot::Qual ? PathReplaySlot::Skills : PathReplaySlot::Qual);
    status_print(std::string("Record slot ") + path_recorder_slot_label(active_slot),
                 std::string("Q:") + path_recorder_storage_label(PathReplaySlot::Qual) +
                     " S:" + path_recorder_storage_label(PathReplaySlot::Skills));
  }

  if (master.get_digital(DIGITAL_L1) && master.get_digital_new_press(DIGITAL_UP)) {
    start_recording();
  }

  if (master.get_digital(DIGITAL_L1) && master.get_digital_new_press(DIGITAL_DOWN)) {
    stop_recording();
  }

  if (recording) {
    sample_path();
  }
}

bool path_recorder_load(std::vector<RecordedPathFrame>& frames) {
  return path_recorder_load(active_slot, frames);
}

bool path_recorder_load(PathReplaySlot slot, std::vector<RecordedPathFrame>& frames) {
  frames.clear();

  const LoadStatus sd_status = load_from_file(slot, frames);
  if (sd_status == LoadStatus::Valid) {
    return true;
  }

  if (ram_path_valid(slot)) {
    frames = frames_for_slot(slot);
    status_print("Using RAM replay", std::to_string(frames.size()) + " frames");
    return true;
  }

  if (sd_status == LoadStatus::OldFormat) {
    status_print("Old replay file", "Re-record this slot");
    return false;
  }
  if (sd_status == LoadStatus::TooShort) {
    status_print("Replay too short", "Record again");
    return false;
  }

  status_print(std::string("No ") + path_recorder_slot_label(slot), "Record one first");
  return false;
}

bool path_recorder_saved_path_exists() {
  return path_recorder_saved_path_exists(active_slot);
}

bool path_recorder_saved_path_exists(PathReplaySlot slot) {
  return path_recorder_sd_path_exists(slot) || path_recorder_ram_path_exists(slot);
}

bool path_recorder_sd_path_exists() {
  return path_recorder_sd_path_exists(active_slot);
}

bool path_recorder_sd_path_exists(PathReplaySlot slot) {
  const PathMetadata& metadata = sd_metadata_get(slot);
  return metadata.checked && metadata.status == LoadStatus::Valid;
}

bool path_recorder_ram_path_exists() {
  return path_recorder_ram_path_exists(active_slot);
}

bool path_recorder_ram_path_exists(PathReplaySlot slot) {
  return ram_path_valid(slot);
}

int path_recorder_duration_ms(PathReplaySlot slot) {
  const PathMetadata& sd_metadata = sd_metadata_get(slot);
  if (sd_metadata.status == LoadStatus::Valid) {
    return sd_metadata.duration_ms;
  }
  if (ram_path_valid(slot)) {
    return frames_for_slot(slot).back().time_ms;
  }
  return 0;
}

int path_recorder_frame_count(PathReplaySlot slot) {
  const PathMetadata& sd_metadata = sd_metadata_get(slot);
  if (sd_metadata.status == LoadStatus::Valid) {
    return sd_metadata.frame_count;
  }
  if (ram_path_valid(slot)) {
    return static_cast<int>(frames_for_slot(slot).size());
  }
  return 0;
}

const char* path_recorder_storage_label() {
  return path_recorder_storage_label(active_slot);
}

const char* path_recorder_storage_label(PathReplaySlot slot) {
  if (path_recorder_sd_path_exists(slot)) {
    return "SD";
  }
  if (path_recorder_ram_path_exists(slot)) {
    return "RAM";
  }
  return "MISSING";
}

const char* path_recorder_slot_label(PathReplaySlot slot) {
  return slot == PathReplaySlot::Skills ? "SKILLS" : "QUAL";
}

void path_recorder_active_slot_set(PathReplaySlot slot) {
  if (!recording) {
    active_slot = slot;
  }
}

PathReplaySlot path_recorder_active_slot_get() {
  return active_slot;
}

double voltage_scale_get() {
  const double voltage = std::clamp(static_cast<double>(pros::battery::get_voltage()), 11000.0, 12600.0);
  return std::clamp(12000.0 / voltage, 1.0, 1.08);
}

int lean_limit_get() {
  const double lean = std::max(std::fabs(chassis.imu.get_pitch()), std::fabs(chassis.imu.get_roll()));
  if (lean >= kTipHardAngle) {
    return 48;
  }
  if (lean >= kTipSlowAngle) {
    return kMaxLeanSlowdown;
  }
  return 127;
}

int scaled_command(int command, double voltage_scale, int lean_limit) {
  const int scaled = static_cast<int>(std::round(command * voltage_scale));
  return std::clamp(scaled, -lean_limit, lean_limit);
}

int sign_of(int input) {
  return input > 0 ? 1 : (input < 0 ? -1 : 0);
}

int right_replay_calibrate(int input, int limit) {
  if (input == 0) {
    return 0;
  }

  int output = static_cast<int>(std::round(input * kRightReplayScale));
  if (kRightReplayMinMovingPower > 0 && std::abs(output) < kRightReplayMinMovingPower) {
    output = sign_of(output) * kRightReplayMinMovingPower;
  }
  return std::clamp(output, -limit, limit);
}

double encoder_polarity(const std::vector<RecordedPathFrame>& frames, bool left_side) {
  double evidence = 0.0;
  for (std::size_t i = 1; i < frames.size(); ++i) {
    const int command = left_side ? frames[i - 1].left : frames[i - 1].right;
    const double previous = left_side ? frames[i - 1].left_position : frames[i - 1].right_position;
    const double current = left_side ? frames[i].left_position : frames[i].right_position;
    const double delta = current - previous;

    if (std::abs(command) >= 12 && std::fabs(delta) >= 0.5) {
      evidence += command * delta;
    }
  }

  return evidence < 0.0 ? -1.0 : 1.0;
}

int distance_correction(double target, double current) {
  const double error = target - current;
  if (std::fabs(error) < kDistanceDeadbandDeg) {
    return 0;
  }
  return std::clamp(static_cast<int>(std::round(error * kDistanceKp)), -kMaxDistanceCorrection, kMaxDistanceCorrection);
}

bool side_is_moving(int command) {
  return std::abs(command) >= kReplayMovingCommand;
}

bool frame_is_moving(const RecordedPathFrame& frame) {
  return side_is_moving(frame.left) || side_is_moving(frame.right);
}

void replay_recorded_path(PathReplaySlot slot, bool mirrored) {
  std::vector<RecordedPathFrame> frames;
  if (!path_recorder_load(slot, frames)) {
    return;
  }

  chassis.pid_targets_reset();
  chassis.drive_sensor_reset();
  chassis.odom_xyt_set(0_in, 0_in, 0_deg);
  chassis.drive_brake_set(MOTOR_BRAKE_HOLD);

  status_print(std::string(mirrored ? "Mirror " : "Replay ") + path_recorder_slot_label(slot),
               std::to_string(frames.size()) + " frames");

  const int replay_start = pros::millis();
  int last_left = 0;
  int last_right = 0;
  const bool has_position_data = frames_have_position_data(frames);
  const double left_polarity = encoder_polarity(frames, true);
  const double right_polarity = encoder_polarity(frames, false);
  master.print(2, 0, "%s", has_position_data ? "Replay v4 corrected" : "Old replay data");
  stop_drive();

  for (const RecordedPathFrame& frame : frames) {
    while (pros::millis() - replay_start < frame.time_ms) {
      pros::delay(1);
    }

    if (!frame_is_moving(frame)) {
      last_left = 0;
      last_right = 0;
      stop_drive();
      continue;
    }

    int left_target = mirrored ? frame.right : frame.left;
    int right_target = mirrored ? frame.left : frame.right;
    left_target = scrub_still_command(left_target);
    right_target = scrub_still_command(right_target);

    const bool left_side_commanded = side_is_moving(left_target);
    const bool right_side_commanded = side_is_moving(right_target);

    const double target_heading = mirrored ? -frame.heading : frame.heading;
    const double heading_error = wrapped_angle_error(target_heading, chassis.odom_theta_get());
    const int correction =
        std::fabs(heading_error) < kHeadingDeadbandDeg
            ? 0
            : std::clamp(static_cast<int>(std::round(heading_error * kHeadingKp)), -kMaxTurnCorrection, kMaxTurnCorrection);

    if (left_side_commanded && right_side_commanded) {
      left_target -= correction;
      right_target += correction;
    } else if (left_side_commanded) {
      left_target -= correction;
    } else if (right_side_commanded) {
      right_target += correction;
    }

    if (has_position_data) {
      const double target_left_position = mirrored ? frame.right_position * right_polarity : frame.left_position * left_polarity;
      const double target_right_position = mirrored ? frame.left_position * left_polarity : frame.right_position * right_polarity;
      const double current_left_position = left_drive_position() * left_polarity;
      const double current_right_position = right_drive_position() * right_polarity;
      if (left_side_commanded) {
        left_target += distance_correction(target_left_position, current_left_position);
      }
      if (right_side_commanded) {
        right_target += distance_correction(target_right_position, current_right_position);
      }
    }

    if (!left_side_commanded) {
      left_target = 0;
    }
    if (!right_side_commanded) {
      right_target = 0;
    }

    const double voltage_scale = voltage_scale_get();
    const int lean_limit = lean_limit_get();
    left_target = scaled_command(left_target, voltage_scale, lean_limit);
    right_target = scaled_command(right_target, voltage_scale, lean_limit);
    right_target = right_replay_calibrate(right_target, lean_limit);

    last_left = left_side_commanded ? approach(last_left, left_target, kReplaySlewStep) : 0;
    last_right = right_side_commanded ? approach(last_right, right_target, kReplaySlewStep) : 0;

    move_side(chassis.left_motors, last_left);
    move_side(chassis.right_motors, last_right);
  }

  stop_drive();
  chassis.drive_brake_set(MOTOR_BRAKE_HOLD);
  pros::delay(350);
  stop_drive();
  status_print("AUTON COMPLETE", std::string(path_recorder_slot_label(slot)) + " replay done");
}

void replay_recorded_path_auton() {
  replay_recorded_path(PathReplaySlot::Qual, false);
}

void replay_skills_path_auton() {
  replay_recorded_path(PathReplaySlot::Skills, false);
}

void replay_mirrored_recorded_path_auton() {
  replay_recorded_path(PathReplaySlot::Qual, true);
}

void replay_mirrored_skills_path_auton() {
  replay_recorded_path(PathReplaySlot::Skills, true);
}
