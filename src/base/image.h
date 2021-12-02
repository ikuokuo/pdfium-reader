#pragma once

#include <cstdint>
#include <iostream>
#include <memory>
#include <vector>

// #define MY_IMAGE_BYTES_SIMPLE
#ifndef MY_IMAGE_BYTES_SIMPLE
class ImageBytes;
#else
using ImageBytes = std::vector<std::uint8_t>;
#endif

class Image {
 public:
  enum Format {
    Gray,   // Gray scale, 1 byte per pixel
    BGR,    // BGR 8:8:8, 3 bytes per pixel
    BGRA,   // BGRA 8:8:8:8, 4 bytes per pixel
    RGB,    // RGB 8:8:8, 3 bytes per pixel
    RGBA,   // RGBA 8:8:8:8, 4 bytes per pixel
  };

  Image();
  Image(int width, int height, Format format);
  ~Image();

  Image(Image &&o);
  Image(const Image &o);
  Image &operator=(Image &&o);
  Image &operator=(const Image &o);

  int width() const { return width_; }
  int height() const { return height_; }
  Format format() const { return format_; }
  int stride() const { return stride_; }
  std::shared_ptr<ImageBytes> bytes() const { return bytes_; }

  std::size_t dimen() const { return width_ * height_; }
  std::size_t size() const;
  std::uint8_t *data();
  const std::uint8_t *data() const;

  static int GetBytesPerPixel(const Format &format);
  static const char *GetString(const Format &format);

 private:
  int width_;
  int height_;
  Format format_;
  int stride_;
  std::shared_ptr<ImageBytes> bytes_;
};

std::ostream &operator<<(std::ostream &, const Image::Format &);

#ifndef MY_IMAGE_BYTES_SIMPLE

class ImageBytes {
 public:
  using value_type = std::uint8_t;
  using size_type = std::size_t;

  explicit ImageBytes(size_type n);
  ~ImageBytes();

  ImageBytes(ImageBytes &&o);
  ImageBytes(const ImageBytes &o);
  ImageBytes &operator=(ImageBytes &&o);
  ImageBytes &operator=(const ImageBytes &o);

  size_type size() const { return bytes_.size(); }
  value_type *data() { return bytes_.data(); }
  const value_type *data() const { return bytes_.data(); }

  value_type operator[](size_type pos) const { return bytes_[pos]; }

  class Iterator {
   public:
    Iterator(const ImageBytes &bytes, size_type index)
      : bytes_(bytes), index_(index) {
    }

    value_type operator*() const { return bytes_[index_]; }
    Iterator &operator++() { index_++; return *this; }  // NOLINT
    bool operator==(const Iterator &o) const {
      return index_ == o.index_;
    }
    bool operator!=(const Iterator &o) const {
      return !(*this == o);
    }
   private:
    const ImageBytes &bytes_;
    size_type index_;
  };

  Iterator begin() const { return Iterator(*this, 0); }
  Iterator end() const { return Iterator(*this, size()); }

 private:
  std::vector<value_type> bytes_;
};

#endif

inline std::size_t Image::size() const { return bytes_->size(); }
inline std::uint8_t *Image::data() { return bytes_->data(); }
inline const std::uint8_t *Image::data() const { return bytes_->data(); }
