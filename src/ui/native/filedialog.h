#pragma once

#include <string>
#include <vector>

enum class DialogCode {
  kError,
  kAccepted,
  kRejected,
};

struct FileFilter {
  std::string name;
  std::string spec;
};

class FileDialog {
 public:
  FileDialog();
  ~FileDialog();

  std::string GetDir() const;
  void SetDir(std::string &&dir);
  void SetDir(const std::string &dir);

  std::vector<FileFilter> GetFilters() const;
  void SetFilters(std::vector<FileFilter> &&filters);
  void SetFilters(const std::vector<FileFilter> &filters);

  DialogCode Open();

  std::string GetError() const;
  std::string GetPath() const;

 protected:
  void OnError(std::string error);
  void OnAccept(std::string path);
  void OnReject();

 private:
  std::string dir_;
  std::vector<FileFilter> filters_;

  std::string error_;
  std::string path_;
};
