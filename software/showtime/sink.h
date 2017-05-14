#pragma once

#include "showtime/types.h"

namespace showtime {

class Sink {
public:
  Sink() : active_(false) {}
  virtual ~Sink() {}
  virtual void sink(ColorChannels channels) = 0;

  void setActive(bool active) { active_ = active; }
  bool active() const { return active_; }

private:
  bool active_;
};

} // namespace showtime
