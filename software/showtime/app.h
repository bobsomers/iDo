#pragma once

#include <memory>
#include <vector>

#include <nanogui/nanogui.h>

//#include "showtime/visualizer.h"
#include "showtime/sink.h"

namespace showtime {

namespace ng = nanogui;

using SinkPtr = std::unique_ptr<Sink>;
using Sinks = std::vector<SinkPtr>;

class App : public ng::Screen {
public:
  explicit App(GLFWwindow* screen);
  void drawContents() override;
  bool keyboardEvent(int key, int scancode, int action, int modifiers) override;

private:
  Sinks sinks_;
};

} // namespace showtime
