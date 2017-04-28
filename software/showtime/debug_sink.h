#pragma once

#include "showtime/sink.h"
#include "showtime/types.h"

namespace showtime {

class DebugSink : public Sink {
public:
  void sink(ColorChannels channels) override;
};

} // namespace showtime
