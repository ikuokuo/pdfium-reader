#include <sstream>
#include "ui/ui.h"
#include "util/log.h"

int main(int argc, char const *argv[]) {
  (void)argc;
  Log::Init(argv[0]);

  if (argc >= 3) {
    UTIL_LOG_V = 2;
  }

  std::stringstream ss;
  ss << MY_APPNAME << " " << MY_VERSION_STR;
  GlfwOptions options{1280, 720, ss.str()};

  if (argc >= 2) {
    return Ui(argv[1]).Run(options);
  } else {
    return Ui().Run(options);
  }
}
