#include "chase_effect.h"

#include "utils.h"

namespace {

std::array<int, 26> CHASE_LOOKUP = {{
  1, 3, 5, 7, 8, 6, 4, 2,
  9, 11, 13, 15, 16, 14, 12, 10,
  17, 19, 21, 23, 24, 22, 20, 18,
  25, 26,
}};

} // namespace

namespace showtime {

ChaseEffect::ChaseEffect(double delay) {
  which_ = 0;
  clr_ = randomColor();
  c = clear();
  c[which_] = clr_;
  next_time_ = -1.0;
  delay_ = delay;
}

ColorChannels ChaseEffect::process(double t_seconds, void* /*data*/) {
  if (next_time_ < 0.0) {
    next_time_ = t_seconds + delay_;
    return c;
  }

  if (t_seconds > next_time_) {
    which_ = (which_ + 1) % 26;
    if (which_ == 0) {
      clr_ = randomColor();
    }
    clear();
    c[CHASE_LOOKUP[which_] - 1] = clr_;
    next_time_ = t_seconds + delay_;
  }

  return c;
}

} // namespace showtime
