#include "showtime/all_off_effect.h"

namespace showtime {

ColorChannels AllOffEffect::process(double, void*) {
  ColorChannels c;
  for (std::size_t i = 0; i < c.size(); ++i) {
    c[i].r = 0.0f;
    c[i].g = 0.0f;
    c[i].b = 0.0f;
  }
  return c;
}

} // namespace showtime
