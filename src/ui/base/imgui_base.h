#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif

#include "glfw_base.h"

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
