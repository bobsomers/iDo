#pragma once

#include "showtime/effect.h"
#include "showtime/types.h"

namespace showtime {

class ColorFadeEffect final : public Effect {
public:
  explicit ColorFadeEffect(double delay);
  ColorChannels process(double t_seconds, void* data) override;

private:
  ColorChannels c;
  double next_time_;
  double delay_;
};

} // namespace showtime
