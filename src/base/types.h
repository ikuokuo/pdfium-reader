#pragma once

#include <cmath>
#include <limits>

struct Scale;
struct Size;
struct SizeF;

struct Scale {
  float x;
  float y;
};

struct Size {
  int w;
  int h;

  SizeF operator*(Scale s) const noexcept;
  bool operator==(const Size &o) const noexcept;
};

struct SizeF {
  float w;
  float h;

  Size ToSize() const noexcept;
  SizeF operator*(Scale s) const noexcept;
  bool operator==(const SizeF &o) const noexcept;
};

inline SizeF Size::operator*(Scale s) const noexcept {
  return {w * s.x, h * s.y};
}

inline bool Size::operator==(const Size &o) const noexcept {
  return w == o.w && h == o.h;
}

inline Size SizeF::ToSize() const noexcept {
  return Size{static_cast<int>(std::round(w)),
              static_cast<int>(std::round(h))};
}

inline SizeF SizeF::operator*(Scale s) const noexcept {
  return {w * s.x, h * s.y};
}

inline bool SizeF::operator==(const SizeF &o) const noexcept {
  return std::fabs(w - o.w) <= std::numeric_limits<float>::epsilon() &&
      std::fabs(h - o.h) <= std::numeric_limits<float>::epsilon();
}
