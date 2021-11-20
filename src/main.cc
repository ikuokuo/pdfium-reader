#include <iostream>

#include "global.h"
#include "ui/ui.h"

int main(int argc, char const *argv[]) {
  (void)argc;
  (void)argv;
  std::cout << MY_APPNAME << " " << MY_VERSION_STR << std::endl;
  return Ui().Run();
}
