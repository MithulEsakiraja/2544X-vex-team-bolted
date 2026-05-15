#include "brain_theme.hpp"

#include <cmath>
#include <cstdint>
#include <string>

#include "main.h"

namespace {
constexpr std::uint32_t kBlack = 0x000000;
constexpr std::uint32_t kTan = 0xEADCC5;
constexpr std::uint32_t kTanDark = 0xB99D75;
constexpr std::uint32_t kWhite = 0xFFFFFF;
constexpr int kScreenW = 480;
constexpr int kScreenH = 240;

int frame = 0;
int last_draw_ms = 0;

void clear(std::uint32_t color = kBlack) {
  pros::screen::set_eraser(color);
  pros::screen::erase();
}

void text(int x, int y, const char* value) {
  pros::screen::set_pen(kTan);
  pros::c::screen_print_at(TEXT_MEDIUM, x, y, "%s", value);
}

void small_text(int x, int y, const char* value) {
  pros::screen::set_pen(kWhite);
  pros::c::screen_print_at(TEXT_SMALL, x, y, "%s", value);
}

void draw_bolt_logo(int cx, int cy, int radius, bool glow) {
  pros::screen::set_pen(glow ? kWhite : kTan);
  pros::screen::fill_circle(cx, cy, radius + (glow ? 3 : 0));
  pros::screen::set_pen(kTan);
  pros::screen::fill_circle(cx, cy, radius);

  pros::screen::set_pen(kBlack);
  pros::screen::draw_line(cx - 8, cy - 26, cx + 11, cy - 26);
  pros::screen::draw_line(cx - 18, cy - 18, cx + 17, cy - 18);
  pros::screen::draw_line(cx - 12, cy - 18, cx - 8, cy - 26);
  pros::screen::draw_line(cx + 12, cy - 18, cx + 8, cy - 26);

  pros::screen::draw_line(cx, cy - 25, cx - 7, cy + 4);
  pros::screen::draw_line(cx - 7, cy + 4, cx + 8, cy + 2);
  pros::screen::draw_line(cx + 8, cy + 2, cx - 2, cy + 31);
  pros::screen::draw_line(cx - 2, cy + 31, cx + 2, cy + 8);
  pros::screen::draw_line(cx + 2, cy + 8, cx - 12, cy + 10);

  pros::screen::draw_line(cx - 8, cy + 16, cx + 6, cy + 11);
  pros::screen::draw_line(cx - 10, cy + 23, cx + 4, cy + 18);
}

void draw_header() {
  pros::screen::set_pen(kTan);
  pros::screen::fill_rect(0, 0, kScreenW, 8);
  pros::screen::fill_rect(0, kScreenH - 8, kScreenW, kScreenH);
}

void draw_speed_lines(int offset) {
  pros::screen::set_pen(kTanDark);
  for (int i = 0; i < 7; ++i) {
    const int y = 44 + i * 24;
    const int left = (offset + i * 37) % 140;
    pros::screen::draw_line(0, y, 74 - left, y);
    pros::screen::draw_line(kScreenW, y + 12, kScreenW - 74 + left, y + 12);
  }
}
}  // namespace

void brain_theme_boot_show() {
  clear();
  draw_header();

  for (int pulse = 0; pulse < 4; ++pulse) {
    clear();
    draw_header();
    draw_speed_lines(pulse * 28);
    draw_bolt_logo(240, 94, 49, pulse % 2 == 0);
    text(157, 162, "2544X BOLTED");
    small_text(163, 190, "BLACKOUT AUTON SYSTEM ONLINE");
    pros::delay(180);
  }

  clear();
  draw_header();
  draw_bolt_logo(240, 94, 52, true);
  text(157, 162, "2544X BOLTED");
  small_text(185, 190, "READY TO LAUNCH");
  pros::delay(650);
}

void brain_theme_screensaver_iterate() {
  const int now = pros::millis();
  if (now - last_draw_ms < 250) {
    return;
  }
  last_draw_ms = now;
  ++frame;

  clear();
  draw_header();
  draw_speed_lines((frame * 17) % 140);

  const int bob = static_cast<int>(std::sin(frame * 0.45) * 6.0);
  draw_bolt_logo(84, 88 + bob, 42, frame % 8 < 4);

  text(168, 58, "TEAM 2544X");
  text(169, 94, "BOLTED");
  const std::string status = std::string("Mode ") + driver_assist_mode_name() +
                             " | Profile " + driver_assist_profile_name() +
                             " | Path " + (path_recorder_saved_path_exists() ? "OK" : "MISSING");
  small_text(169, 130, status.c_str());

  const std::string pose = "X " + util::to_string_with_precision(chassis.odom_x_get(), 1) +
                           "  Y " + util::to_string_with_precision(chassis.odom_y_get(), 1) +
                           "  A " + util::to_string_with_precision(chassis.odom_theta_get(), 1);
  small_text(169, 158, pose.c_str());

  pros::screen::set_pen(kTanDark);
  const int sweep = 24 + ((frame * 19) % 432);
  pros::screen::draw_line(sweep, 216, sweep + 28, 216);
  pros::screen::draw_line(sweep + 40, 216, sweep + 72, 216);
}
