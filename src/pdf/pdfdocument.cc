#include "pdfdocument.h"
#include "pdfdocument_p.h"

#include <vector>

#include "fpdf_doc.h"
#include "fpdf_scopers.h"
#include "fpdf_text.h"

#include "util/log.h"
#include "util/strings.h"

namespace pdf {

namespace {

int kLibraryRefCount{0};

}  // namespace

// DocumentPrivate

DocumentPrivate::DocumentPrivate() {
  VLOG(2) << __func__;
  Locker lock;
  if (kLibraryRefCount == 0) {
    VLOG(1) << "FPDF_InitLibrary";
    FPDF_InitLibrary();
  }
  ++kLibraryRefCount;
}

DocumentPrivate::~DocumentPrivate() {
  VLOG(2) << __func__;
  Close();

  Locker lock;
  if (!--kLibraryRefCount) {
    VLOG(1) << "FPDF_DestroyLibrary";
    FPDF_DestroyLibrary();
  }
}

void DocumentPrivate::UpdateStatus(Status status_) {
  if (status == status_) return;
  status = status_;
  q->OnStatusChanged(status);
}

void DocumentPrivate::UpdateLastError(Error error) {
  if (last_error == error) return;
  last_error = error;
  q->OnErrorOccurred(error);
}

void DocumentPrivate::UpdateLastError() {
  if (doc) {
    last_error = kNoError;
    return;
  }

  Locker lock;
  const unsigned long error = FPDF_GetLastError();
  lock.unlock();

  switch (error) {
    case FPDF_ERR_SUCCESS: last_error = kNoError; break;
    case FPDF_ERR_UNKNOWN: last_error = kUnknownError; break;
    case FPDF_ERR_FILE: last_error = kFileNotFoundError; break;
    case FPDF_ERR_FORMAT: last_error = kInvalidFileFormatError; break;
    case FPDF_ERR_PASSWORD: last_error = kIncorrectPasswordError; break;
    case FPDF_ERR_SECURITY: last_error = kUnsupportedSecuritySchemeError; break;
    default: LOG(FATAL) << "unreachable";
  }

  UpdateLastError(last_error);
}

void DocumentPrivate::Load(const std::string &filename) {
  LOG_IF(FATAL, doc != nullptr) << "load already, close firstly";

  doc = FPDF_LoadDocument(filename.c_str(), password.c_str());

  if (!doc) {
    UpdateLastError();
    UpdateStatus(kError);
    if (last_error == kIncorrectPasswordError) {
      q->OnPasswordRequired();
    }
    return;
  }

  Locker lock;
  const int page_count_new = FPDF_GetPageCount(doc);
  lock.unlock();
  if (page_count != page_count_new) {
    page_count = page_count_new;
    q->OnPageCountChanged(page_count);
  }

  UpdateStatus(kReady);
}

void DocumentPrivate::Close() {
  if (!doc) return;
  UpdateStatus(kUnloading);
  {
    Locker lock;
    if (doc)
      FPDF_CloseDocument(doc);
    doc = nullptr;
  }
  if (!password.empty()) {
    password.clear();
    q->OnPasswordChanged();
  }
  if (page_count != 0) {
    page_count = 0;
    q->OnPageCountChanged(page_count);
  }
  UpdateStatus(kNull);
}

// Document

Document::Document() : d{new DocumentPrivate} {
  d->q = this;
}

Document::~Document() {
  delete d;
}

Error Document::Load(const std::string &filename) {
  Close();
  d->UpdateStatus(kLoading);
  d->Load(filename);
  return d->last_error;
}

Status Document::GetStatus() const {
  return d->status;
}

Error Document::GetError() const {
  return d->last_error;
}

std::string Document::GetPassword() const {
  return d->password;
}

void Document::SetPassword(const std::string &password) {
  if (d->password == password) return;
  d->password = password;
  OnPasswordChanged();
}

std::string Document::GetMetaData(MetaDataField field) const {
  if (!d->doc) return "";

  std::string name;
  switch (field) {
    case kTitle: name = "Title"; break;
    case kSubject: name = "Subject"; break;
    case kAuthor: name = "Author"; break;
    case kKeywords: name = "Keywords"; break;
    case kProducer: name = "Producer"; break;
    case kCreator: name = "Creator"; break;
    case kCreationDate: name = "CreationDate"; break;
    case kModificationDate: name = "ModDate"; break;
  }

  Locker lock;
  const unsigned long len = FPDF_GetMetaText(d->doc, name.c_str(), nullptr, 0);
  std::vector<char16_t> buf(len);
  FPDF_GetMetaText(d->doc, name.c_str(), buf.data(), buf.size());
  lock.unlock();

  auto text = strings::from_utf16(std::u16string(buf.data()));
  if (name == "CreationDate" || name == "ModDate") {
    text = DateToRFC3399(text);
  }

  return text;
}

int Document::GetPageCount() const {
  return d->page_count;
}

SizeF Document::GetPageSize(int page) const {
  SizeF result;
  if (!d->doc)
    return result;

  Locker lock;
  FS_SIZEF size;
  FPDF_GetPageSizeByIndexF(d->doc, page, &size);
  result.w = size.width;
  result.h = size.height;
  return result;
}

std::shared_ptr<Image> Document::Render(int page_i, Size page_size) {
  if (!d->doc)
    return nullptr;
  if (page_size.w <= 0 || page_size.h <= 0)
    return nullptr;

  const Locker lock;

  FPDF_PAGE page = FPDF_LoadPage(d->doc, page_i);
  if (!page)
    return nullptr;

  int alpha = FPDFPage_HasTransparency(page) ? 1 : 0;

  auto image = std::make_shared<Image>(page_size.w, page_size.h, Image::BGRA);

  FPDF_BITMAP bitmap = FPDFBitmap_CreateEx(
      image->width(), image->height(), FPDFBitmap_BGRA,
      image->data(), image->stride());

  FPDF_DWORD fill_color = alpha ? 0x00000000 : 0xFFFFFFFF;
  FPDFBitmap_FillRect(bitmap, 0, 0,
      image->width(), image->height(), fill_color);

  int rotation = 0;
  int flags = FPDF_ANNOT;
  FPDF_RenderPageBitmap(bitmap, page, 0, 0,
      image->width(), image->height(), rotation, flags);

  FPDFBitmap_Destroy(bitmap);
  FPDF_ClosePage(page);
  return image;
}

void Document::Close() {
  d->Close();
}

}  // namespace pdf
