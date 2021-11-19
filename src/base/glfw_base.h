#pragma once

#include <memory>
#include <string>

#include "glm/vec4.hpp"

#include "glfw_base_types.h"

struct GLFWwindow;

class GlfwBase {
 public:
  using Callback = std::shared_ptr<GlfwBaseCallback>;

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
      GlfwRunCallback callback = nullptr);

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
