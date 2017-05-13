#pragma once

#include "showtime/types.h"

namespace showtime {

class Sink {
public:
  virtual ~Sink() {}
  virtual void sink(ColorChannels channels) = 0;
};

} // namespace showtime
