#pragma once

#define UTIL_LOG_TAG ""
#include "logger.h"

struct Log {
  static void Init(const char* argv0) {
    (void)argv0;
    UTIL_LOG_PREFIX = true;
    UTIL_LOG_MINLEVEL = Logger::INFO;
    UTIL_LOG_V = 0;
  }
};
