#pragma once

enum class SelectedAuton {
  Qual,
  Skills,
  QualMirror,
  SkillsMirror,
};

void competition_ui_initialize();
void competition_ui_iterate();
void competition_ui_selected_auton_call();
const char* competition_ui_selected_auton_name();
bool competition_ui_selected_auton_ready();
int competition_ui_selected_auton_duration_ms();
