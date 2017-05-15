#pragma once

#include <netinet/in.h>

#include "showtime/sink.h"
#include "showtime/types.h"

namespace showtime {

class NetworkSink : public Sink {
public:
  NetworkSink();
  ~NetworkSink() override;

  void sink(ColorChannels channels) override;

private:
  int socket_;
  sockaddr_in addr_;
  ByteColorChannels packet_;
  double last_packet_time_;
};

} // namespace showtime
