#pragma once

#include <string>
#include <memory>

#include "base/image.h"
#include "base/types.h"
#include "pdfevent.h"

namespace pdf {

class DocumentPrivate;

class Document : public EventManager {
 public:
  Document();
  ~Document() override;

  Error Load(const std::string &filename);

  Status GetStatus() const;
  Error GetError() const;

  std::string GetPassword() const;
  void SetPassword(const std::string &password);

  std::string GetMetaData(MetaDataField field) const;

  int GetPageCount() const;
  SizeF GetPageSize(int page) const;

  std::shared_ptr<Image> Render(int page, Size page_size);

  void Close();

 protected:
  DocumentPrivate *d;

  friend class DocumentPrivate;
};

}  // namespace pdf

// ref: QPdfDocument
//  https://code.qt.io/cgit/qt/qtwebengine.git/tree/src/pdf/qpdfdocument.h
