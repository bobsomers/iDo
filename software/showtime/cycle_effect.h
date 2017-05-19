#pragma once

#include "showtime/effect.h"
#include "showtime/types.h"

namespace showtime {

class CycleEffect final : public Effect {
public:
  explicit CycleEffect(double min_t, double max_t);
  ColorChannels process(double t_seconds, void* data) override;

private:
  using EffectPtr = std::unique_ptr<Effect>;

  void randomEffect();

  double next_time_;
  double min_t_;
  double max_t_;
  EffectPtr effect_;
  int which_;
};

} // namespace showtime
