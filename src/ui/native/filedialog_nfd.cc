#include "filedialog.h"

#include <utility>

#include "nfd.h"

FileDialog::FileDialog() {
}

FileDialog::~FileDialog() {
}

std::string FileDialog::GetDir() const {
  return dir_;
}

void FileDialog::SetDir(std::string &&dir) {
  dir_ = std::move(dir);
}

void FileDialog::SetDir(const std::string &dir) {
  dir_ = dir;
}

std::vector<FileFilter> FileDialog::GetFilters() const {
  return filters_;
}

void FileDialog::SetFilters(std::vector<FileFilter> &&filters) {
  filters_ = std::move(filters);
}

void FileDialog::SetFilters(const std::vector<FileFilter> &filters) {
  filters_ = filters;
}

DialogCode FileDialog::Open() {
  path_.clear();
  error_.clear();

  nfdchar_t *outPath;
  nfdfilteritem_t *filterList = NULL;
  nfdfiltersize_t filterCount = 0;
  nfdchar_t *defaultPath = NULL;

  if (!dir_.empty()) {
    defaultPath = dir_.data();
  }
  if (!filters_.empty()) {
    filterCount = filters_.size();
    filterList = new nfdfilteritem_t[filterCount];
    for (nfdfiltersize_t i = 0; i < filterCount; i++) {
      filterList[i].name = filters_[i].name.data();
      filterList[i].spec = filters_[i].spec.data();
    }
  }

  nfdresult_t result = NFD_OpenDialog(
      &outPath, filterList, filterCount, defaultPath);

  auto code = DialogCode::kError;
  if (result == NFD_OKAY) {
    path_ = outPath;
    OnAccept(path_);
    NFD_FreePath(outPath);
    code = DialogCode::kAccepted;
  } else if (result == NFD_CANCEL) {
    OnReject();
    code = DialogCode::kRejected;
  } else {
    error_ = NFD_GetError();
    OnError(error_);
    code = DialogCode::kError;
  }

  if (filterList != NULL) {
    delete[] filterList;
  }
  return code;
}

std::string FileDialog::GetError() const {
  return error_;
}

std::string FileDialog::GetPath() const {
  return path_;
}

void FileDialog::OnError(std::string error) {
  (void)error;
}

void FileDialog::OnAccept(std::string path) {
  (void)path;
}

void FileDialog::OnReject() {
}
