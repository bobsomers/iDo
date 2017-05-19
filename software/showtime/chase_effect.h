#pragma once

#include "showtime/effect.h"
#include "showtime/types.h"

namespace showtime {

class ChaseEffect final : public Effect {
public:
  explicit ChaseEffect(double delay);
  ColorChannels process(double t_seconds, void* data) override;

private:
  ColorChannels c;
  double next_time_;
  double delay_;
  int which_;
  Color clr_;
};

} // namespace showtime
