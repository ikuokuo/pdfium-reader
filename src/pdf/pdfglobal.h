#pragma once

#include <iostream>
#include <mutex>
#include <string>

#include "global.h"

// types

namespace pdf {

enum Status {
  kNull,
  kLoading,
  kReady,
  kUnloading,
  kError,
};
const char *GetString(const Status &);

enum Error {
  kNoError,
  kUnknownError,
  kDataNotYetAvailableError,
  kFileNotFoundError,
  kInvalidFileFormatError,
  kIncorrectPasswordError,
  kUnsupportedSecuritySchemeError,
};
const char *GetString(const Error &);

enum MetaDataField {
  kTitle,
  kSubject,
  kAuthor,
  kKeywords,
  kProducer,
  kCreator,
  kCreationDate,
  kModificationDate,
};
const char *GetString(const MetaDataField &);

enum EventId {
  kErrorOccurred,
  kPasswordChanged,
  kPasswordRequired,
  kStatusChanged,
  kPageCountChanged,
};
const char *GetString(const EventId &);

}  // namespace pdf

std::ostream &operator<<(std::ostream &, const pdf::Status &);
std::ostream &operator<<(std::ostream &, const pdf::Error &);
std::ostream &operator<<(std::ostream &, const pdf::MetaDataField &);
std::ostream &operator<<(std::ostream &, const pdf::EventId &);

// helper

namespace pdf {

std::string DateToRFC3399(const std::string &str);

}  // namespace pdf

// locker

namespace pdf {

class Locker : public std::unique_lock<std::recursive_mutex> {
 public:
  Locker() : std::unique_lock<std::recursive_mutex>(*Mutex()) {}

  std::recursive_mutex *Mutex() {
    static std::recursive_mutex mutex{};
    return &mutex;
  }
};

}  // namespace pdf
