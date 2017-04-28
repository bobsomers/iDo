#include "test_effect.h"

#include <iostream>

namespace showtime {

ColorChannels TestEffect::process(double t_seconds, void* /*data*/) {
  ColorChannels c;

  const int max_channels = c.size() * 3;
  const int second = static_cast<int>(t_seconds);
  const int current_channel = second % max_channels;
  const int index = current_channel / 3;
  const int rgb = current_channel - (index * 3);
  float intensity = 1.0f - (t_seconds - second);

  switch (rgb) {
  case 0:
    c[index].r = intensity;
    break;

  case 1:
    c[index].g = intensity;
    break;

  case 2:
    c[index].b = intensity;
    break;

  default:
    std::cerr << "UH OH." << std::endl;
  }

  return c;
}

} // namespace showtime
