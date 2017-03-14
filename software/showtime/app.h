#pragma once

#include <memory>

#include "showtime/visualizer.h"

namespace showtime {

namespace ng = nanogui;

class App : public ng::Screen {
public:
  explicit App(GLFWwindow* screen);
  void drawContents() override;
  bool keyboardEvent(int key, int scancode, int action, int modifiers) override;

private:
  std::unique_ptr<Visualizer> visualizer_ = nullptr;
};

} // namespace showtime
