#pragma once

#include "base/imgui_base.h"

class Ui : public ImGuiBase {
 public:
  Ui();
  ~Ui() override;

 protected:
  void OnCreate() override;
  void OnDestroy() override;

  void OnDraw() override;

 private:
  void ShowMainMenuBar();
  void ShowMenuFile();

  void OpenFileDialog();
};
