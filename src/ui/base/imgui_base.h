#pragma once

#include "glfw_base.h"
#include "imgui.hpp"

class ImGuiBase : public GlfwBase {
 public:
  ImGuiBase();
  ~ImGuiBase() override;

 protected:
  void OnWindowCreateBefore() override;

  void OnCreate() override;
  void OnDrawPre() override;
  void OnDraw() override;
  void OnDrawPost() override;
  void OnDestroy() override;
};
