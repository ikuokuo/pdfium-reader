#pragma once

#include <memory>
#include <string>
#include <vector>

#include "base/imgui_base.h"
#include "pdf/pdfdocument.h"

class Page;

class Ui : public ImGuiBase {
 public:
  explicit Ui(std::string path = "");
  ~Ui() override;

 protected:
  void OnCreate() override;
  void OnDestroy() override;

  void OnDraw() override;

 private:
  void ShowMainMenuBar();
  void ShowMenuFile();

  void FitPdfPages();

  void OpenFileDialog();
  void OpenFile(const std::string &path);

  void OnPdfEvent(const std::shared_ptr<pdf::Event> &e);

  void OnViewportSizeChanged(const ImGuiViewport *viewport);

  std::string arg_path_;

  std::shared_ptr<pdf::Document> pdf_;
  std::vector<std::shared_ptr<Page>> pdf_pages_;

  float left_pane_width_;
  ImVec2 left_pane_size_;
  ImVec2 right_pane_size_;
};
