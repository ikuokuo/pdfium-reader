#pragma once

#include <string>
#include <memory>

#include "fpdfview.h"
#include "fpdf_dataavail.h"

#include "pdfdocument.h"

namespace pdf {

class DocumentPrivate {
 public:
  DocumentPrivate();
  ~DocumentPrivate();

  Document *q;

  FPDF_DOCUMENT doc{nullptr};

  std::string password{};

  Status status{kNull};
  Error last_error{kNoError};
  int page_count{0};

  void UpdateStatus(Status status);
  void UpdateLastError(Error error);
  void UpdateLastError();

  void Load(const std::string &filename);
  void Close();
};

}  // namespace pdf
