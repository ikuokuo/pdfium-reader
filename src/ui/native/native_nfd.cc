#include "native.h"
#include "nfd.h"

#include "util/log.h"

class NFD : public Native::Module {
 public:
  void Create() override {
    if (NFD_Init() != NFD_OKAY) {
      LOG(ERROR) << "NFD_Init failed";
    }
  }

  void Destroy() override {
    NFD_Quit();
  }
};

NATIVE_REGISTER_MODULE(NFD)
