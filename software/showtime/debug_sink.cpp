#include "showtime/debug_sink.h"

#include <iostream>

namespace showtime {

void DebugSink::sink(ColorChannels channels) {
  std::cout << "=== CHANNELS ========================\n";
  ByteColorChannels quantized = quantizeChannels(channels);
  for (std::size_t i = 0; i < quantized.size(); ++i) {
    int r = quantized[i].r;
    int g = quantized[i].g;
    int b = quantized[i].b;
    std::cout << i << ": <" << r << ", " << g << ", " << b << ">\n";
  }
  std::cout << std::flush;
}

} // namespace showtime
