#include "ui.h"

#include <functional>
#include <utility>

// #define STB_IMAGE_WRITE_IMPLEMENTATION
// #include "stb_image_write.h"

#include "ui/native/filedialog.h"
#include "ui/native/native.h"
#include "ui/page.h"
#include "util/log.h"
#include "util/numbers.h"

Ui::Ui(std::string path)
  : ImGuiBase::ImGuiBase(),
    arg_path_(std::move(path)), pdf_{new pdf::Document},
    left_pane_width_{150}, left_pane_size_{0, 0}, right_pane_size_{0, 0} {
  pdf_->SetEventCallback(
      std::bind(&Ui::OnPdfEvent, this, std::placeholders::_1));
}

Ui::~Ui() {
  if (pdf_) {
    pdf_->Close();
    pdf_ = nullptr;
  }
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

  const ImGuiViewport *viewport = ImGui::GetMainViewport();
  bool view_size_changed = false;
  static auto view_size = viewport->Size;
  if (!numbers::equalf(view_size.x, viewport->Size.x) ||
      !numbers::equalf(view_size.y, viewport->Size.y)) {
    view_size_changed = true;
    view_size = viewport->Size;
  }

  auto &style = ImGui::GetStyle();

  ImGui::SetNextWindowPos(viewport->WorkPos);
  ImGui::SetNextWindowSize(viewport->WorkSize);
  ImGui::Begin("Main", 0, ImGuiWindowFlags_NoTitleBar |
      ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
      ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground |
      ImGuiWindowFlags_NoSavedSettings);

  bool do_render_pages = !view_size_changed;
  // render
  if (do_render_pages) {
    for (auto page : pdf_pages_) {
      auto sz = page->FitSize();
      // render_size = fit_size * 2
      auto render_size = (sz * Scale{2, 2}).ToSize();
      page->Render(render_size);
    }
  }
  // left
  static int selected_page_index = 0;
  bool do_select = false;
  {
    ImGui::BeginChild("left", ImVec2(left_pane_width_, 0), true);
    left_pane_size_ = ImGui::GetContentRegionAvail();
    if (do_render_pages) {
      for (int i = 0, n = pdf_pages_.size(); i < n; i++) {
        auto page = pdf_pages_[i];
        auto tex = page->GetTexture();
        if (!tex) continue;
        auto sz = page->FitSize();

        int frame_padding = -1;  // default padding
        auto w = left_pane_size_.x - style.FramePadding.x*2;
        ImVec2 size = ImVec2(w, sz.h * w / sz.w);
        ImVec2 uv0 = ImVec2(0.0f, 0.0f);
        ImVec2 uv1 = ImVec2(1.0f, 1.0f);
        bool selected = (selected_page_index == i);
        if (selected) {
          ImGui::PushID(i);
          ImGui::PushStyleColor(ImGuiCol_Button,
              style.Colors[ImGuiCol_ButtonActive]);
        }
        if (ImGui::ImageButton((void*)(intptr_t)tex->Id(),  // NOLINT
            size, uv0, uv1, frame_padding)) {
          selected_page_index = i;
          do_select = true;
        }
        if (selected) {
          ImGui::PopStyleColor(1);
          ImGui::PopID();
        }
      }
    }
    ImGui::EndChild();
  }
  ImGui::SameLine();
  // right
  {
    ImGui::BeginChild("right", ImVec2(0, 0));
    right_pane_size_ = ImGui::GetContentRegionAvail();
    if (do_render_pages) {
      ImVec2 selected_pos;
      for (int i = 0, n = pdf_pages_.size(); i < n; i++) {
        auto page = pdf_pages_[i];
        auto tex = page->GetTexture();
        if (!tex) continue;
        auto sz = page->FitSize();
        if (do_select && selected_page_index == i) {
          selected_pos = ImGui::GetCursorPos();
        }
        ImGui::SetCursorPosX((right_pane_size_.x - sz.w) * 0.5f);
        ImGui::Image((void*)(intptr_t)tex->Id(), ImVec2(sz.w, sz.h));  // NOLINT
      }
      if (do_select) {
        ImGui::SetScrollY(selected_pos.y);
      }
    }
    ImGui::EndChild();
  }
  ImGui::End();

  if (!arg_path_.empty()) {
    OpenFile(arg_path_);
    arg_path_.clear();
  }

  if (view_size_changed) {
    OnViewportSizeChanged(viewport);
  }
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

  /*
  if (ImGui::BeginMenu("Open Recent")) {
    ImGui::MenuItem("fish_hat.c");
    ImGui::MenuItem("fish_hat.inl");
    ImGui::MenuItem("fish_hat.h");
    ImGui::Separator();
    ImGui::MenuItem("Clear Recently Opened");
    ImGui::EndMenu();
  }
  */

  ImGui::Separator();
  if (ImGui::MenuItem("Quit", "Alt+F4")) {
    Quit();
  }
}

void Ui::FitPdfPages() {
  pdf_pages_.clear();

  if (pdf_->GetStatus() != pdf::kReady) {
    return;
  }

  if (right_pane_size_.x <= 0 || right_pane_size_.y <= 0) {
    return;
  }

  // always fit pages to work area now
  for (auto i = 0, n = pdf_->GetPageCount(); i < n; i++) {
    auto page = std::make_shared<Page>(pdf_, i);
    page->FitTo(right_pane_size_.x, right_pane_size_.y);
    pdf_pages_.push_back(page);

    if (VLOG_IS_ON(1)) {
      auto size = page->RawSize();
      auto fit_size = page->FitSize();
      VLOG(1) << " " << i << ", " << size.w << "x" << size.h
          << ", fit=" << fit_size.w << "x" << fit_size.h;
    }
  }
}

void Ui::OpenFileDialog() {
  auto dialog = FileDialog{};
  // dialog.SetDir("/Users/John/Downloads");
  dialog.SetFilters({{"PDF", "pdf"}});
  auto code = dialog.Open();
  if (code == DialogCode::kAccepted) {
    auto path = dialog.GetPath();
    LOG(INFO) << "File open accepted: " << path;
    OpenFile(path);
  } else if (code == DialogCode::kRejected) {
    LOG(INFO) << "File open rejected";
  } else {
    LOG(ERROR) << "File open error: " << dialog.GetError();
  }
}

void Ui::OpenFile(const std::string &path) {
  using namespace pdf;  // NOLINT

  if (pdf_) pdf_->Close();
  if (pdf_->Load(path) != kNoError) {
    return;
  }

  if (VLOG_IS_ON(1)) {
    VLOG(1) << "MetaData";
    VLOG(1) << " Title=\"" << pdf_->GetMetaData(kTitle) << "\"";
    VLOG(1) << " Subject=\"" << pdf_->GetMetaData(kSubject) << "\"";
    VLOG(1) << " Author=\"" << pdf_->GetMetaData(kAuthor) << "\"";
    VLOG(1) << " Keywords=\"" << pdf_->GetMetaData(kKeywords) << "\"";
    VLOG(1) << " Producer=\"" << pdf_->GetMetaData(kProducer) << "\"";
    VLOG(1) << " Creator=\"" << pdf_->GetMetaData(kCreator) << "\"";
    VLOG(1) << " CreationDate=\"" << pdf_->GetMetaData(kCreationDate) << "\"";
    VLOG(1) << " ModificationDate=\""
              << pdf_->GetMetaData(kModificationDate) << "\"";

    VLOG(1) << "PageCount=" << pdf_->GetPageCount();
    // for (auto i = 0, n = pdf_->GetPageCount(); i < n; i++) {
    //   auto size = pdf_->GetPageSize(i);
    //   auto image = pdf_->Render(i, size.ToSize());
    //   std::stringstream ss;
    //   ss << i << ".png";
    //   stbi_write_png(ss.str().c_str(), image->width(), image->height(),
    //       4, image->data(), image->stride());
    // }
  }

  FitPdfPages();
}

void Ui::OnPdfEvent(const std::shared_ptr<pdf::Event> &e) {
  using namespace pdf;  // NOLINT
  switch (e->id) {
    case kErrorOccurred: {
      auto event = std::dynamic_pointer_cast<EventErrorOccurred>(e);
      VLOG(1) << "EventErrorOccurred, " << event->error;
    } break;
    case kPasswordChanged: {
      auto event = std::dynamic_pointer_cast<EventPasswordChanged>(e);
      VLOG(1) << "EventPasswordChanged" << pdf_->GetPassword();
    } break;
    case kPasswordRequired: {
      auto event = std::dynamic_pointer_cast<EventPasswordRequired>(e);
      VLOG(1) << "EventPasswordRequired";
    } break;
    case kStatusChanged: {
      auto event = std::dynamic_pointer_cast<EventStatusChanged>(e);
      VLOG(1) << "EventStatusChanged, " << event->status;
    } break;
    case kPageCountChanged: {
      auto event = std::dynamic_pointer_cast<EventPageCountChanged>(e);
      VLOG(1) << "EventPageCountChanged, " << event->page_count;
    } break;
  }
}

void Ui::OnViewportSizeChanged(const ImGuiViewport *viewport) {
  VLOG(1) << "Viewport size=" << viewport->Size.x << "x" << viewport->Size.y;
  FitPdfPages();
}
