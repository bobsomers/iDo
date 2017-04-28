#pragma once

#include "showtime/types.h"

namespace showtime {

class Effect {
public:
  // TODO: Replace void* data with audio data type.
  virtual ColorChannels process(void* data) = 0;
};

} // namespace showtime
