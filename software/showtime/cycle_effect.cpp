#include "cycle_effect.h"

#include <cstdlib>

#include "chase_effect.h"
#include "color_fade_effect.h"
#include "utils.h"

namespace showtime {

void CycleEffect::randomEffect() {
  int which = std::rand() % 2;
  switch (which) {
  case 0:
    effect_.reset(new ColorFadeEffect(0.1));
    break;

  case 1:
    effect_.reset(new ChaseEffect(0.2));
    break;

  default:
    effect_.reset(new ChaseEffect(0.2));
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
