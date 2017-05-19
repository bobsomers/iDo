#include "cycle_effect.h"

#include <cstdlib>

#include "all_on_effect.h"
#include "all_off_effect.h"
#include "color_fade_effect.h"
#include "test_effect.h"

namespace showtime {

double randomDelay(double min, double max) {
  return (static_cast<float>(std::rand()) / RAND_MAX) * (max - min) + min;
}

void CycleEffect::randomEffect() {
  int which = std::rand() % 3;
  switch (which) {
  case 0:
    effect_.reset(new AllOnEffect);
    break;

  case 1:
    effect_.reset(new TestEffect);
    break;

  case 2:
    effect_.reset(new ColorFadeEffect(0.1));
    break;

  default:
    effect_.reset(new AllOffEffect);
    break;
  }
}

CycleEffect::CycleEffect(double min_t, double max_t) {
  next_time_ = -1.0;
  min_t_ = min_t;
  max_t_ = max_t;
}

ColorChannels CycleEffect::process(double t_seconds, void* data) {
  if (next_time_ < 0.0) {
    next_time_ = t_seconds + randomDelay(min_t_, max_t_);
    randomEffect();
  }

  if (t_seconds > next_time_) {
    randomEffect();
    next_time_ = t_seconds + randomDelay(min_t_, max_t_);
  }

  return effect_->process(t_seconds, data);;
}

} // namespace showtime
