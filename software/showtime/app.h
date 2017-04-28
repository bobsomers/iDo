#pragma once

#include <memory>
#include <vector>

#include <nanogui/nanogui.h>

#include "showtime/effect.h"
#include "showtime/sink.h"

namespace showtime {

namespace ng = nanogui;

using EffectPtr = std::unique_ptr<Effect>;
using SinkPtr = std::unique_ptr<Sink>;
using Sinks = std::vector<SinkPtr>;

class App : public ng::Screen {
public:
  explicit App(GLFWwindow* screen);
  void drawContents() override;
  bool keyboardEvent(int key, int scancode, int action, int modifiers) override;

private:
  void createEffectsWindow();
  void setEffect(Effect* new_effect);

  EffectPtr effect_ = nullptr;
  double effect_t_start_ = 0.0;
  Sinks sinks_;
};

} // namespace showtime
