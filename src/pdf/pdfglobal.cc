#include "pdfglobal.h"

#include "util/strings.h"

// types

namespace pdf {

const char *GetString(const Status &status) {
#define CASE(X) case Status::X: return #X;
  switch (status) {
    CASE(kNull)
    CASE(kLoading)
    CASE(kReady)
    CASE(kUnloading)
    CASE(kError)
    default: return MY_UNKNOWN;
  }
#undef CASE
}

const char *GetString(const Error &error) {
#define CASE(X) case Error::X: return #X;
  switch (error) {
    CASE(kNoError)
    CASE(kUnknownError)
    CASE(kDataNotYetAvailableError)
    CASE(kFileNotFoundError)
    CASE(kInvalidFileFormatError)
    CASE(kIncorrectPasswordError)
    CASE(kUnsupportedSecuritySchemeError)
    default: return MY_UNKNOWN;
  }
#undef CASE
}

const char *GetString(const MetaDataField &field) {
#define CASE(X) case MetaDataField::X: return #X;
  switch (field) {
    CASE(kTitle)
    CASE(kSubject)
    CASE(kAuthor)
    CASE(kKeywords)
    CASE(kProducer)
    CASE(kCreator)
    CASE(kCreationDate)
    CASE(kModificationDate)
    default: return MY_UNKNOWN;
  }
#undef CASE
}

const char *GetString(const EventId &id) {
#define CASE(X) case EventId::X: return #X;
  switch (id) {
    CASE(kErrorOccurred)
    CASE(kPasswordChanged)
    CASE(kPasswordRequired)
    CASE(kStatusChanged)
    CASE(kPageCountChanged)
    default: return MY_UNKNOWN;
  }
#undef CASE
}

}  // namespace pdf

std::ostream &operator<<(std::ostream &os, const pdf::Status &status) {
  return os << pdf::GetString(status);
}

std::ostream &operator<<(std::ostream &os, const pdf::Error &error) {
  return os << pdf::GetString(error);
}

std::ostream &operator<<(std::ostream &os, const pdf::MetaDataField &field) {
  return os << pdf::GetString(field);
}

std::ostream &operator<<(std::ostream &os, const pdf::EventId &id) {
  return os << pdf::GetString(id);
}

// types

namespace pdf {

std::string DateToRFC3399(const std::string &str) {
  if (str.empty()) return str;
  std::string s = str;
  if (strings::starts_with(s, "D:")) {
    s.erase(0, 2);
  }
  s.insert(4, "-");
  s.insert(7, "-");
  s.insert(10, "T");
  s.insert(13, ":");
  s.insert(16, ":");
  strings::replace_all(s, "'", ":");
  if (strings::ends_with(s, ":")) {
    s.erase(s.size()-1);
  }
  return s;
}

}  // namespace pdf
