#pragma once

#include "showtime/sink.h"
#include "showtime/types.h"

namespace showtime {

class NetworkSink : public Sink {
public:
  void sink(ColorChannels channels) override;
};

} // namespace showtime
