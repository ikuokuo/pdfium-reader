#include "image.h"

#include <list>
#include <mutex>
#include <utility>

#include "global.h"
#include "util/log.h"

namespace {

class ImageBytesCache {
 private:
  ImageBytesCache() {}

 public:
  ~ImageBytesCache() {}

  static ImageBytesCache &Instance() {
    static ImageBytesCache instance;
    return instance;
  }

  std::shared_ptr<ImageBytes> Get(ImageBytes::size_type n) {
    if (n == 0) return std::make_shared<ImageBytes>(n);
    std::lock_guard<std::mutex> _(mutex_);
    // put back to caches if only one used
    for (auto it = caches_used_.begin(); it != caches_used_.end(); ) {
      if (it->use_count() > 1)  {
        ++it;
      } else {
        caches_.push_back(std::move(*it));
        it = caches_used_.erase(it);
      }
    }
    // get form caches
    std::shared_ptr<ImageBytes> bytes = nullptr;
    for (auto it = caches_.begin(); it != caches_.end(); ) {
      if ((*it)->size() == n) {
        bytes = std::move(*it);
        it = caches_.erase(it);
      } else {
        ++it;
      }
    }
    // create one
    if (!bytes) {
      bytes = std::make_shared<ImageBytes>(n);
    }
    caches_used_.push_back(bytes);
    return bytes;
  }

 private:
  std::mutex mutex_;

  std::list<std::shared_ptr<ImageBytes>> caches_;
  std::list<std::shared_ptr<ImageBytes>> caches_used_;

  MY_DISABLE_COPY_MOVE(ImageBytesCache)
};

}  // namespace

// Image

Image::Image() : Image(0, 0, Format::BGRA) {
}

Image::Image(int width, int height, Format format)
  : width_(width), height_(height), format_(format) {
  VLOG(2) << __func__ << ": " << width << "x" << height << ", " << format;
  stride_ = width * GetBytesPerPixel(format);
  bytes_ = std::make_shared<ImageBytes>(height * stride_);
  // bytes_ = ImageBytesCache::Instance().Get(height * stride_);
}

Image::~Image() {
  VLOG(2) << __func__;
}

Image::Image(Image &&o)
  : width_(std::exchange(o.width_, 0)),
    height_(std::exchange(o.height_, 0)),
    format_(std::exchange(o.format_, Format::BGRA)),
    stride_(std::exchange(o.stride_, 0)),
    bytes_(std::move(o.bytes_)) {
  VLOG(2) << __func__ << ", move";
}

Image::Image(const Image &o)
  : width_(o.width_),
    height_(o.height_),
    format_(o.format_),
    stride_(o.stride_),
    bytes_(o.bytes_) {
  VLOG(2) << __func__ << ", copy";
}

Image &Image::operator=(Image &&o) {
  VLOG(2) << __func__ << ", move";
  width_ = std::exchange(o.width_, 0);
  height_ = std::exchange(o.height_, 0);
  format_ = std::exchange(o.format_, Format::BGRA);
  stride_ = std::exchange(o.stride_, 0);
  bytes_ = std::move(o.bytes_);
  return *this;
}

Image &Image::operator=(const Image &o) {
  VLOG(2) << __func__ << ", copy";
  width_ = o.width_;
  height_ = o.height_;
  format_ = o.format_;
  stride_ = o.stride_;
  bytes_ = o.bytes_;
  return *this;
}

int Image::GetBytesPerPixel(const Format &format) {
  switch (format) {
    case Format::Gray:  return 1;
    case Format::BGR:   return 3;
    case Format::BGRA:  return 4;
    case Format::RGB:   return 3;
    case Format::RGBA:  return 4;
    default:
      LOG(FATAL) << "Unaccepted format";
      return 0;
  }
}

const char *Image::GetString(const Format &format) {
#define CASE(X) case Format::X: return #X;
  switch (format) {
    CASE(Gray)
    CASE(BGR)
    CASE(BGRA)
    CASE(RGB)
    CASE(RGBA)
    default: return MY_UNKNOWN;
  }
#undef CASE
}

std::ostream &operator<<(std::ostream &os, const Image::Format &format) {
  return os << Image::GetString(format);
}

// ImageBytes

#ifndef MY_IMAGE_BYTES_SIMPLE

ImageBytes::ImageBytes(size_type n) : bytes_(n) {
  VLOG(2) << __func__ << ": " << n;
}

ImageBytes::~ImageBytes() {
  VLOG(2) << __func__;
}

ImageBytes::ImageBytes(ImageBytes &&o)
  : bytes_(std::move(o.bytes_)) {
  VLOG(2) << __func__<< ", move";
}

ImageBytes::ImageBytes(const ImageBytes &o)
  : bytes_(o.bytes_) {
  VLOG(2) << __func__<< ", copy";
}

ImageBytes &ImageBytes::operator=(ImageBytes &&o) {
  VLOG(2) << __func__<< ", move";
  bytes_ = std::move(o.bytes_);
  return *this;
}

ImageBytes &ImageBytes::operator=(const ImageBytes &o) {
  VLOG(2) << __func__<< ", copy";
  bytes_ = o.bytes_;
  return *this;
}

#endif
