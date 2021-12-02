#pragma once

#include <memory>

#include "base/image.h"
#include "base/types.h"
#include "pdf/pdfdocument.h"
#include "ui/gl/texture.h"

class Page {
 public:
  Page(const std::shared_ptr<pdf::Document> &pdf, int index);
  ~Page();

  int Index() const;
  SizeF RawSize() const;

  void FitTo(int w, int h);
  void FitToWidth(int w);
  void FitToHeight(int h);

  Size FitSize() const;
  Scale FitScale() const;

  bool Render(bool force = false);
  bool Render(Size size, bool force = false);
  void RenderClear();
  std::shared_ptr<Image> GetImage();
  std::shared_ptr<Texture> GetTexture();

 private:
  std::shared_ptr<pdf::Document> pdf_;
  int index_;
  SizeF size_;
  Size fit_size_;

  Size render_size_;
  std::shared_ptr<Image> render_img_;
  std::shared_ptr<Texture> render_tex_;
};
