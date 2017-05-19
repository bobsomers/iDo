#include "color_fade_effect.h"

#include <cstdlib>
#include <iostream>

namespace showtime {

Color randomColor() {
  Color c;
  c.r = (static_cast<float>(std::rand()) / RAND_MAX) * 0.75f + 0.25f;
  c.g = (static_cast<float>(std::rand()) / RAND_MAX) * 0.75f + 0.25f;
  c.b = (static_cast<float>(std::rand()) / RAND_MAX) * 0.75f + 0.25f;
  return c;
}

ColorFadeEffect::ColorFadeEffect(double delay) {
  for (int i = 0; i < 26; ++i) {
    c[i] = randomColor();
  }
  next_time_ = -1.0;
  delay_ = delay;
}

ColorChannels ColorFadeEffect::process(double t_seconds, void* /*data*/) {
  if (next_time_ < 0.0) {
    next_time_ = t_seconds + delay_;
    return c;
  }

  if (t_seconds > next_time_) {
    int which = std::rand() % 26;
    c[which] = randomColor();
    next_time_ = t_seconds + delay_;
  }

  return c;
}

} // namespace showtime
