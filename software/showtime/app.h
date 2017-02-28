#pragma once

#include "nanogui/nanogui.h"

#include "showtime/table.h"

namespace showtime {

namespace ng = nanogui;

class App : public ng::Screen {
public:
  App();
  void drawContents() override;
  bool keyboardEvent(int key, int scancode, int action, int modifiers) override;

private:
  Table table_;
};

} // namespace showtime
