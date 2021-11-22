#include "glfw_base.h"

#include <cassert>
#include <iostream>

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif
#include <GLFW/glfw3.h>

namespace {

void glfw_error_callback(int error, const char *description) {
  std::cerr << "Glfw Error " << error << ": " << description << std::endl;
}

void glfw_framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  (void)window;
  glViewport(0, 0, width, height);
}

}  // namespace

GlfwBase::GlfwBase()
  : window_(nullptr),
    callback_(nullptr),
    quit_(false),
    clear_color_(0.f, 0.f, 0.f, 1.f) {
}

GlfwBase::~GlfwBase() {
}

GLFWwindow *GlfwBase::Create(const GlfwOptions &options_) {
  if (window_) return window_;
  auto options = DefaultOptions(options_);

  // Setup window
  glfwSetErrorCallback(glfw_error_callback);
  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW" << std::endl;
    return nullptr;
  }

  if (callback_) {
    if (!callback_->IsWindowCreateBeforeOverride(this)) {
      OnWindowCreateBefore();
    }
    callback_->OnWindowCreateBefore(this);
  } else {
    OnWindowCreateBefore();
  }

  // Create window with graphics context
  GLFWwindow *window = glfwCreateWindow(
      options.width, options.height, options.title.c_str(), NULL, NULL);
  if (window == NULL) {
    std::cerr << "Failed to open GLFW window" << std::endl;
    return nullptr;
  }

  window_ = window;
  if (callback_) {
    if (!callback_->IsWindowCreateAfterOverride(this, window)) {
      OnWindowCreateAfter(window);
    }
    callback_->OnWindowCreateAfter(this, window);
  } else {
    OnWindowCreateAfter(window);
  }

  OnCreate();
  if (callback_) callback_->OnCreate(this);
  return window_;
}

bool GlfwBase::ShouldClose(bool accept_key_escape) {
  assert(window_);
  return glfwWindowShouldClose(window_) || quit_ ||
      (accept_key_escape && glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS);
}

void GlfwBase::Draw() {
  assert(window_);
  if (callback_) {
    if (callback_->IsDrawOverride(this)) {
      callback_->OnDraw(this);
    } else {
      OnDrawPre();
      OnDraw();
      callback_->OnDraw(this);
      OnDrawPost();
    }
  } else {
    OnDrawPre();
    OnDraw();
    OnDrawPost();
  }
}

void GlfwBase::Destroy() {
  if (!window_) return;

  OnDestroy();
  if (callback_) callback_->OnDestory(this);

  glfwDestroyWindow(window_);
  glfwTerminate();
  window_ = nullptr;
}

void GlfwBase::Quit() {
  quit_ = true;
}

GLFWwindow *GlfwBase::GetWindow() const {
  return window_;
}

std::string GlfwBase::GetGLSLVersion() const {
  return glsl_version_;
}

glm::vec4 GlfwBase::GetClearColor() const {
  return clear_color_;
}

void GlfwBase::SetClearColor(const glm::vec4 &color) {
  clear_color_ = color;
}

void GlfwBase::SetCallback(Callback callback) {
  callback_ = callback;
}

int GlfwBase::Run(const GlfwOptions &options, GlfwRunCallback callback) {
  GLFWwindow *glfw_window = Create(options);
  if (!glfw_window) return 1;

  while (!ShouldClose()) {
    if (callback) callback(this);
    Draw();
  }

  Destroy();
  return 0;
}

void GlfwBase::OnWindowCreateBefore() {
  // Decide GL+GLSL versions
  glfwWindowHint(GLFW_SAMPLES, 4);
#if __APPLE__
  // GL 3.2 + GLSL 150
  const char *glsl_version = "#version 150";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
  // GL 3.0 + GLSL 130
  const char *glsl_version = "#version 130";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
  // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif
  glsl_version_ = glsl_version;
}

void GlfwBase::OnWindowCreateAfter(GLFWwindow *window) {
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, glfw_framebuffer_size_callback);
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
  glfwSwapInterval(1);  // Enable vsync
}

void GlfwBase::OnCreate() {
}

void GlfwBase::OnDrawPre() {
  glClearColor(clear_color_.r, clear_color_.g, clear_color_.b, clear_color_.a);
  glClear(GL_COLOR_BUFFER_BIT);
}

void GlfwBase::OnDraw() {
}

void GlfwBase::OnDrawPost() {
  glfwSwapBuffers(window_);
  // Poll and handle events (inputs, window resize, etc.)
  // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
  // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
  // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
  // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
  glfwPollEvents();
}

void GlfwBase::OnDestroy() {
}

GlfwOptions GlfwBase::DefaultOptions(const GlfwOptions &options_) {
  GlfwOptions options = options_;
  if (options.width <= 0) options.width = 1280;
  if (options.height <= 0) options.height = 720;
  if (options.title.empty()) options.title = "Window";
  return options;
}
