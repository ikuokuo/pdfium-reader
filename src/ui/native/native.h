#pragma once

#include <memory>
#include <utility>
#include <vector>

struct Native {
  class Module {
   public:
    virtual ~Module() = default;
    virtual void Create() = 0;
    virtual void Destroy() = 0;
  };

  static void Create() {
    auto modules = Modules();
    for (auto m = modules.begin(); m != modules.end(); ++m) {
      (*m)->Create();
    }
  }

  static void Destroy() {
    auto modules = Modules();
    for (auto m = modules.rbegin(); m != modules.rend(); ++m) {
      (*m)->Destroy();
    }
  }

  static bool Register(std::shared_ptr<Module> m) {
    Modules().push_back(std::move(m));
    return true;
  }

 private:
  static std::vector<std::shared_ptr<Module>> &Modules() {
    static std::vector<std::shared_ptr<Module>> modules;
    return modules;
  }
};

#define NATIVE_REGISTER_MODULE(T) \
  const bool kIsNativeRegisterModule_##T = \
    Native::Register(std::make_shared<T>());
