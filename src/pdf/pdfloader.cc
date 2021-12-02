#include "pdfloader.h"

#include <utility>

namespace pdf {

// FileLoader

FileLoader::FileLoader(std::string path) : path_(std::move(path)) {
}

FileLoader::~FileLoader() {
  if (IsOpen()) Close();
}

void FileLoader::Open() {
  input_ = std::ifstream(path_,
      std::ios::binary | std::ios::in | std::ios::ate);
}

void FileLoader::Close() {
  input_.close();
}

bool FileLoader::IsOpen() const {
  return input_.is_open();
}

size_t FileLoader::Size() {
  return input_.tellg();
}

size_t FileLoader::Read(char *data, size_t max_size) {
  return input_.read(data, max_size).gcount();
}

bool FileLoader::Seek(size_t pos) {
  return !!input_.seekg(pos);
}

}  // namespace pdf
