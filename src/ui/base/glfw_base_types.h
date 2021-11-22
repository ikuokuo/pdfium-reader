#pragma once

#include <functional>
#include <string>

class GlfwBase;
struct GLFWwindow;
class GlfwBaseCallback {
 public:
  virtual ~GlfwBaseCallback() = default;

  virtual void OnWindowCreateBefore(GlfwBase *) {}
  virtual void OnWindowCreateAfter(GlfwBase *, GLFWwindow *) {}

  virtual void OnCreate(GlfwBase *) {}
  virtual void OnDraw(GlfwBase *) {}
  virtual void OnDestory(GlfwBase *) {}

  // Return true to override the original behavior
  virtual bool IsWindowCreateBeforeOverride(GlfwBase *) { return false; }
  virtual bool IsWindowCreateAfterOverride(GlfwBase *, GLFWwindow *) {
    return false;
  }
  virtual bool IsDrawOverride(GlfwBase *) { return false; }
};

struct GlfwOptions {
  int width;
  int height;
  std::string title;
};

using GlfwRunCallback = std::function<void(GlfwBase *)>;
