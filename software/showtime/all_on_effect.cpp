#include "showtime/all_on_effect.h"

namespace showtime {

ColorChannels AllOnEffect::process(double, void*) {
  ColorChannels c;
  for (std::size_t i = 0; i < c.size(); ++i) {
    c[i].r = 1.0f;
    c[i].g = 1.0f;
    c[i].b = 1.0f;
  }
  return c;
}

} // namespace showtime
