#pragma once

#include <string>
#include <fstream>

namespace pdf {

class Loader {
 public:
  Loader() = default;
  virtual ~Loader() = default;

  virtual void Open() = 0;
  virtual void Close() = 0;

  virtual bool IsOpen() const = 0;

  virtual size_t Size() = 0;
  virtual size_t Read(char *data, size_t max_size) = 0;
  virtual bool Seek(size_t pos) = 0;
};

class FileLoader : public Loader {
 public:
  explicit FileLoader(std::string path);
  ~FileLoader() override;

  void Open() override;
  void Close() override;

  bool IsOpen() const override;

  size_t Size() override;
  size_t Read(char *data, size_t max_size) override;
  bool Seek(size_t pos) override;

 private:
  std::string path_;
  std::ifstream input_;
};

}  // namespace pdf
