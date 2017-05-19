#include <cstdlib>

namespace showtime {

inline Color randomColor() {
  Color c;
  c.r = (static_cast<float>(std::rand()) / RAND_MAX) * 0.75f + 0.25f;
  c.g = (static_cast<float>(std::rand()) / RAND_MAX) * 0.75f + 0.25f;
  c.b = (static_cast<float>(std::rand()) / RAND_MAX) * 0.75f + 0.25f;
  return c;
}

inline ColorChannels clear() {
  ColorChannels c;
  for (int i = 0; i < 26; ++i) {
    c[i].r = 0.0f;
    c[i].g = 0.0f;
    c[i].b = 0.0f;
  }
  return c;
}

inline double randomDelay(double min, double max) {
  return (static_cast<float>(std::rand()) / RAND_MAX) * (max - min) + min;
}

} // namespace showtime
