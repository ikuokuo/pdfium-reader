#include <iostream>

#include "native.h"
#include "nfd.h"

class NFD : public Native::Module {
 public:
  void Create() override {
    if (NFD_Init() != NFD_OKAY) {
      std::cerr << "NFD_Init failed" << std::endl;
    }
  }

  void Destroy() override {
    NFD_Quit();
  }
};

NATIVE_REGISTER_MODULE(NFD)
