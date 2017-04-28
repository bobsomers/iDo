#pragma once

#include "showtime/effect.h"
#include "showtime/types.h"

namespace showtime {

class AllOnEffect final : public Effect {
public:
  ColorChannels process(double, void*) override;
};

} // namespace showtime
