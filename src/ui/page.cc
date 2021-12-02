#include "page.h"

#include "util/log.h"
#include "util/numbers.h"

Page::Page(const std::shared_ptr<pdf::Document> &pdf, int index)
  : pdf_(pdf), index_(index) {
  LOG_IF(FATAL, pdf_->GetStatus() != pdf::kReady);
  size_ = pdf_->GetPageSize(index_);
  fit_size_ = size_.ToSize();
  RenderClear();
}

Page::~Page() {
  RenderClear();
}

int Page::Index() const {
  return index_;
}

SizeF Page::RawSize() const {
  return size_;
}

void Page::FitTo(int w, int h) {
  LOG_IF(FATAL, w <= 0 || h <= 0);
  if (size_.w/w >= size_.h/h) {
    FitToWidth(w);
  } else {
    FitToHeight(h);
  }
}

void Page::FitToWidth(int w) {
  fit_size_ = Size{w, numbers::roundf(size_.h * w / size_.w)};
}

void Page::FitToHeight(int h) {
  fit_size_ = Size{numbers::roundf(size_.w * h / size_.h), h};
}

Size Page::FitSize() const {
  return fit_size_;
}

Scale Page::FitScale() const {
  return {fit_size_.w/size_.w, fit_size_.h/size_.h};
}

bool Page::Render(bool force) {
  return Render(fit_size_, force);
}

bool Page::Render(Size size, bool force) {
  if (size == render_size_ && !force) {
    return true;  // already rendered
  }
  auto img = pdf_->Render(index_, size);
  if (img == nullptr) {
    return false;
  }
  render_size_ = size;
  render_img_ = img;
  render_tex_ = std::make_shared<Texture>(true);
  GLenum tex_format = GL_RED;
  GLenum pixel_format = GL_RED;
  switch (img->format()) {
    case Image::Gray: {
      tex_format = GL_RED;
      pixel_format = GL_RED;
    } break;
    case Image::BGR: {
      tex_format = GL_RGB;
      pixel_format = GL_BGR;
    } break;
    case Image::BGRA: {
      tex_format = GL_RGBA;
      pixel_format = GL_BGRA;
    } break;
    case Image::RGB: {
      tex_format = GL_RGB;
      pixel_format = GL_RGB;
    } break;
    case Image::RGBA: {
      tex_format = GL_RGBA;
      pixel_format = GL_RGBA;
    } break;
  }
  render_tex_->Fill(tex_format, img->width(), img->height(),
      pixel_format, img->data());
  return true;
}

void Page::RenderClear() {
  render_size_ = {0, 0};
  render_img_.reset();
  render_tex_.reset();
}

std::shared_ptr<Image> Page::GetImage() {
  return render_img_;
}

std::shared_ptr<Texture> Page::GetTexture() {
  return render_tex_;
}
