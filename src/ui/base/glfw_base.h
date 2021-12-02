#pragma once

#include <functional>
#include <memory>
#include <string>

#include "glm/vec4.hpp"

struct GLFWwindow;

struct GlfwOptions {
  int width;
  int height;
  std::string title;
};
class GlfwBaseCallback;

class GlfwBase {
 public:
  using Callback = std::shared_ptr<GlfwBaseCallback>;
  using RunCallback = std::function<void(GlfwBase *)>;

  GlfwBase();
  virtual ~GlfwBase();

  GLFWwindow *Create(const GlfwOptions &options = GlfwOptions{});
  bool ShouldClose(bool accept_key_escape = true);
  void Draw();
  void Destroy();

  void Quit();

  GLFWwindow *GetWindow() const;
  std::string GetGLSLVersion() const;

  glm::vec4 GetClearColor() const;
  void SetClearColor(const glm::vec4 &color);

  void SetCallback(Callback callback);

  virtual int Run(
      const GlfwOptions &options = GlfwOptions{},
      RunCallback callback = nullptr);

 protected:
  virtual void OnWindowCreateBefore();
  virtual void OnWindowCreateAfter(GLFWwindow *);

  virtual void OnCreate();
  virtual void OnDrawPre();
  virtual void OnDraw();
  virtual void OnDrawPost();
  virtual void OnDestroy();

 protected:
  GlfwOptions DefaultOptions(const GlfwOptions &options);

  GLFWwindow *window_;
  std::string glsl_version_;

  Callback callback_;

  bool quit_;
  glm::vec4 clear_color_;
};

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
