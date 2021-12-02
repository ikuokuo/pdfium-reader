#pragma once

#include "base/event.h"
#include "pdfglobal.h"

namespace pdf {

using Event = ::Event<EventId>;

class EventErrorOccurred : public Event {
 public:
  explicit EventErrorOccurred(Error error)
    : Event(kErrorOccurred), error(error) {}
  ~EventErrorOccurred() override = default;
  Error error;
};

class EventPasswordChanged : public Event {
 public:
  EventPasswordChanged() : Event(kPasswordChanged) {}
  ~EventPasswordChanged() override = default;
};

class EventPasswordRequired : public Event {
 public:
  EventPasswordRequired() : Event(kPasswordRequired) {}
  ~EventPasswordRequired() override = default;
};

class EventStatusChanged : public Event {
 public:
  explicit EventStatusChanged(Status status)
    : Event(kStatusChanged), status(status) {}
  ~EventStatusChanged() override = default;
  Status status;
};

class EventPageCountChanged : public Event {
 public:
  explicit EventPageCountChanged(int page_count)
    : Event(kPageCountChanged), page_count(page_count) {}
  ~EventPageCountChanged() override = default;
  int page_count;
};

class EventManager : public ::EventManager<EventId> {
 public:
  EventManager() = default;
  ~EventManager() override = default;

 protected:
  virtual void OnErrorOccurred(Error error) {
    DispatchEvent<EventErrorOccurred>(error);
  }

  virtual void OnPasswordChanged() {
    DispatchEvent<EventPasswordChanged>();
  }

  virtual void OnPasswordRequired() {
    DispatchEvent<EventPasswordRequired>();
  }

  virtual void OnStatusChanged(Status status) {
    DispatchEvent<EventStatusChanged>(status);
  }

  virtual void OnPageCountChanged(int page_count) {
    DispatchEvent<EventPageCountChanged>(page_count);
  }
};

}  // namespace pdf
