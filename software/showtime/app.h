#pragma once

#include <memory>

#include "showtime/table.h"

namespace showtime {

namespace ng = nanogui;

class App : public ng::Screen {
public:
  explicit App(GLFWwindow* screen);
  void drawContents() override;
  bool keyboardEvent(int key, int scancode, int action, int modifiers) override;

private:
  std::unique_ptr<Table> table_ = nullptr;
};

} // namespace showtime
