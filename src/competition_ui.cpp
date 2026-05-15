// 2544X Bolted Brain HUD and auton selector. The UI is part of the team's
// field workflow, not a generic skin.
#include "competition_ui.hpp"

#include <string>

#include "main.h"

namespace {
constexpr int kBlack = 0x050505;
constexpr int kPanel = 0x17120C;
constexpr int kPanelAlt = 0x261E14;
constexpr int kTan = 0xEADCC5;
constexpr int kTanDim = 0x9F8D70;
constexpr int kTanDark = 0x5F503B;
constexpr int kRed = 0xB84A3D;
constexpr int kGreen = 0x42B883;
constexpr int kGold = 0xD6AF55;
constexpr int kTouchPadding = 14;
constexpr int kTouchCooldownMs = 90;
constexpr int kRepairRedrawMs = 2500;

SelectedAuton selected_auton = SelectedAuton::Qual;
int last_touch_ms = 0;
int last_repair_draw_ms = 0;
bool ui_dirty = true;
bool screen_claimed = false;
SelectedAuton last_touched_auton = SelectedAuton::Qual;
PathReplaySlot last_seen_record_slot = PathReplaySlot::Qual;

struct Button {
  int x0;
  int y0;
  int x1;
  int y1;
  const char* label;
  SelectedAuton auton;
};

constexpr Button kAutonButtons[] = {
    {8, 64, 228, 108, "QUAL", SelectedAuton::Qual},
    {252, 64, 472, 108, "SKILLS", SelectedAuton::Skills},
    {8, 120, 228, 164, "Q MIR", SelectedAuton::QualMirror},
    {252, 120, 472, 164, "S MIR", SelectedAuton::SkillsMirror},
};

bool in_button(const pros::screen_touch_status_s_t& touch, const Button& button) {
  return touch.x >= button.x0 - kTouchPadding && touch.x <= button.x1 + kTouchPadding &&
         touch.y >= button.y0 - kTouchPadding && touch.y <= button.y1 + kTouchPadding;
}

const Button* pressed_button_get(const pros::screen_touch_status_s_t& touch) {
  for (const Button& button : kAutonButtons) {
    if (in_button(touch, button)) {
      return &button;
    }
  }
  return nullptr;
}

bool auton_has_path_requirement(SelectedAuton auton) {
  return auton == SelectedAuton::Qual || auton == SelectedAuton::Skills ||
         auton == SelectedAuton::QualMirror || auton == SelectedAuton::SkillsMirror;
}

PathReplaySlot slot_for_auton(SelectedAuton auton) {
  if (auton == SelectedAuton::Skills || auton == SelectedAuton::SkillsMirror) {
    return PathReplaySlot::Skills;
  }
  return PathReplaySlot::Qual;
}

bool auton_is_ready(SelectedAuton auton) {
  return !auton_has_path_requirement(auton) || path_recorder_saved_path_exists(slot_for_auton(auton));
}

int duration_for_auton(SelectedAuton auton) {
  if (!auton_has_path_requirement(auton)) {
    return 0;
  }
  return path_recorder_duration_ms(slot_for_auton(auton));
}

int frame_count_for_auton(SelectedAuton auton) {
  if (!auton_has_path_requirement(auton)) {
    return 0;
  }
  return path_recorder_frame_count(slot_for_auton(auton));
}

const char* battery_label() {
  const double capacity = pros::battery::get_capacity();
  if (capacity < 20.0) {
    return "CRIT";
  }
  if (capacity < 35.0) {
    return "LOW";
  }
  return "OK";
}

bool confidence_ok() {
  const std::string drive_health = driver_assist_drive_health_label();
  return auton_is_ready(selected_auton) && pros::battery::get_capacity() >= 35.0 &&
         drive_health != "HOT" && drive_health != "CHECK";
}

const char* ready_label(PathReplaySlot slot) {
  if (path_recorder_sd_path_exists(slot)) {
    return "SD";
  }
  if (path_recorder_ram_path_exists(slot)) {
    return "RAM";
  }
  return "NO";
}

void draw_outline(int x0, int y0, int x1, int y1, int color) {
  pros::screen::set_pen(color);
  pros::screen::draw_rect(x0, y0, x1, y1);
}

void draw_button(const Button& button) {
  const bool selected = selected_auton == button.auton;
  const bool ready = auton_is_ready(button.auton);
  const int fill = selected ? kPanelAlt : kPanel;

  pros::screen::set_pen(fill);
  pros::screen::fill_rect(button.x0, button.y0, button.x1, button.y1);
  draw_outline(button.x0, button.y0, button.x1, button.y1, selected ? kGold : kTanDark);

  if (selected) {
    pros::screen::set_pen(ready ? kGreen : kRed);
    pros::screen::fill_rect(button.x0, button.y0, button.x0 + 6, button.y1);
  }

  pros::screen::set_pen(selected ? kTan : kTanDim);
  pros::c::screen_print_at(TEXT_MEDIUM, button.x0 + 24, button.y0 + 12, "%s", button.label);
}

void draw_ui() {
  pros::screen::set_pen(kBlack);
  pros::screen::fill_rect(0, 0, 479, 191);

  pros::screen::set_pen(kPanel);
  pros::screen::fill_rect(0, 0, 479, 58);
  pros::screen::set_pen(kTanDark);
  pros::screen::fill_rect(0, 57, 479, 58);
  pros::screen::set_pen(kGold);
  pros::screen::fill_rect(0, 0, 6, 58);

  pros::screen::set_pen(kTan);
  pros::c::screen_print_at(TEXT_MEDIUM, 18, 12, "2544X BOLTED");
  pros::screen::set_pen(kTanDim);
  pros::c::screen_print_at(TEXT_SMALL, 204, 18, "AUTON");
  pros::screen::set_pen(kGold);
  pros::c::screen_print_at(TEXT_SMALL, 346, 18, "RUN %s", competition_ui_selected_auton_name());

  pros::screen::set_pen(kTanDim);
  pros::c::screen_print_at(TEXT_SMALL, 18, 40, "QUAL %s", ready_label(PathReplaySlot::Qual));
  pros::c::screen_print_at(TEXT_SMALL, 118, 40, "SKILLS %s", ready_label(PathReplaySlot::Skills));
  pros::c::screen_print_at(TEXT_SMALL, 238, 40, "REC %s", path_recorder_slot_label(path_recorder_active_slot_get()));
  pros::c::screen_print_at(TEXT_SMALL, 344, 40, "BAT %s", battery_label());
  pros::c::screen_print_at(TEXT_SMALL, 420, 40, "%s", pros::competition::is_connected() ? "FIELD" : "PIT");

  for (const Button& button : kAutonButtons) {
    draw_button(button);
  }

  pros::screen::set_pen(auton_is_ready(selected_auton) ? kGreen : kRed);
  pros::screen::fill_rect(8, 164, 112, 186);
  pros::screen::set_pen(kBlack);
  pros::c::screen_print_at(TEXT_SMALL, 17, 169, "%s", confidence_ok() ? "CONF OK" : "RISK");
  pros::screen::set_pen(kTanDim);
  pros::c::screen_print_at(TEXT_SMALL, 128, 164, "TIME %.1fs   FR %d",
                           duration_for_auton(selected_auton) / 1000.0, frame_count_for_auton(selected_auton));
  pros::c::screen_print_at(TEXT_SMALL, 326, 164, "DRV %s", driver_assist_drive_health_label());
  pros::c::screen_print_at(TEXT_SMALL, 128, 182, "L1+UP start   L1+DOWN save   R2 slot");

  pros::screen::set_pen(kBlack);
  pros::screen::fill_rect(0, 192, 479, 199);

  screen_claimed = true;
}

void touch_iterate() {
  const pros::screen_touch_status_s_t touch = pros::screen::touch_status();
  const bool pressed = touch.touch_status == TOUCH_PRESSED;
  if (!pressed) {
    return;
  }

  if (pros::competition::is_autonomous()) {
    return;
  }

  const Button* touched_button = pressed_button_get(touch);
  if (touched_button != nullptr) {
      const int now = pros::millis();
      if (now - last_touch_ms < kTouchCooldownMs && last_touched_auton == touched_button->auton) {
        return;
      }

      last_touch_ms = now;
      last_touched_auton = touched_button->auton;
      const bool changed = selected_auton != touched_button->auton;
      selected_auton = touched_button->auton;
      if (auton_has_path_requirement(selected_auton)) {
        path_recorder_active_slot_set(slot_for_auton(selected_auton));
      }
      ui_dirty = true;
      if (changed) {
        master.print(0, 0, "Auton: %s", competition_ui_selected_auton_name());
      }
      return;
  }
}
}  // namespace

void competition_ui_initialize() {
  selected_auton = SelectedAuton::Qual;
  ui_dirty = true;
  screen_claimed = false;
  last_touch_ms = 0;
  last_touched_auton = SelectedAuton::Qual;
  path_recorder_active_slot_set(PathReplaySlot::Qual);
  last_seen_record_slot = PathReplaySlot::Qual;
  draw_ui();
  last_repair_draw_ms = pros::millis();
  ui_dirty = false;
}

void competition_ui_iterate() {
  touch_iterate();
  const int now = pros::millis();

  if (last_seen_record_slot != path_recorder_active_slot_get()) {
    last_seen_record_slot = path_recorder_active_slot_get();
    ui_dirty = true;
  }

  if (screen_claimed && now - last_repair_draw_ms >= kRepairRedrawMs) {
    ui_dirty = true;
  }

  if (!ui_dirty && screen_claimed) {
    return;
  }

  ui_dirty = false;
  draw_ui();
  last_repair_draw_ms = now;
}

const char* competition_ui_selected_auton_name() {
  switch (selected_auton) {
    case SelectedAuton::Skills:
      return "SKILLS";
    case SelectedAuton::QualMirror:
      return "Q MIR";
    case SelectedAuton::SkillsMirror:
      return "S MIR";
    case SelectedAuton::Qual:
    default:
      return "QUAL";
  }
}

bool competition_ui_selected_auton_ready() {
  return auton_is_ready(selected_auton);
}

int competition_ui_selected_auton_duration_ms() {
  return duration_for_auton(selected_auton);
}

void competition_ui_selected_auton_call() {
  switch (selected_auton) {
    case SelectedAuton::Skills:
      path_recorder_active_slot_set(PathReplaySlot::Skills);
      replay_skills_path_auton();
      break;
    case SelectedAuton::QualMirror:
      path_recorder_active_slot_set(PathReplaySlot::Qual);
      replay_mirrored_recorded_path_auton();
      break;
    case SelectedAuton::SkillsMirror:
      path_recorder_active_slot_set(PathReplaySlot::Skills);
      replay_mirrored_skills_path_auton();
      break;
    case SelectedAuton::Qual:
    default:
      path_recorder_active_slot_set(PathReplaySlot::Qual);
      replay_recorded_path_auton();
      break;
  }
}
