#pragma once

#include <cmath>
#include <limits>

namespace numbers {

inline int roundf(float f) {
  return static_cast<int>(std::round(f));
}

inline bool equalf(float a, float b) {
  return std::fabs(a - b) <= std::numeric_limits<float>::epsilon();
}

}  // namespace numbers
