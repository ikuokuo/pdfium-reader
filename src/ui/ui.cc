#include "ui.h"

#include <iostream>

#include "native/filedialog.h"
#include "native/native.h"

Ui::Ui() : ImGuiBase::ImGuiBase() {
}

Ui::~Ui() {
}

void Ui::OnCreate() {
  ImGuiBase::OnCreate();
  Native::Create();
}

void Ui::OnDestroy() {
  Native::Destroy();
  ImGuiBase::OnDestroy();
}

void Ui::OnDraw() {
  ShowMainMenuBar();
}

void Ui::ShowMainMenuBar() {
  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu("File")) {
      ShowMenuFile();
      ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
  }
}

void Ui::ShowMenuFile() {
  if (ImGui::MenuItem("Open", "Ctrl+O")) {
    OpenFileDialog();
  }
  if (ImGui::BeginMenu("Open Recent")) {
    ImGui::MenuItem("fish_hat.c");
    ImGui::MenuItem("fish_hat.inl");
    ImGui::MenuItem("fish_hat.h");
    ImGui::Separator();
    ImGui::MenuItem("Clear Recently Opened");
    ImGui::EndMenu();
  }

  ImGui::Separator();
  if (ImGui::MenuItem("Quit", "Alt+F4")) {
    Quit();
  }
}

void Ui::OpenFileDialog() {
  auto dialog = FileDialog{};
  // dialog.SetDir("/Users/John/Downloads");
  dialog.SetFilters({{"PDF", "pdf"}});
  auto code = dialog.Open();
  if (code == DialogCode::kAccepted) {
    auto path = dialog.GetPath();
    std::cout << "File open accepted: " << path << std::endl;
  } else if (code == DialogCode::kRejected) {
    std::cout << "File open rejected" << std::endl;
  } else {
    std::cerr << "File open error: " << dialog.GetError() << std::endl;
  }
}
