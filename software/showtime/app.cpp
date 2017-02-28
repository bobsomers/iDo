#include "showtime/app.h"

#include <iostream>
#include <cstdlib>
#include <unistd.h>

namespace showtime {

App::App()
  : ng::Screen(ng::Vector2i(1280, 720), "Showtime"),
    table_(ng::Vector2i(200, 200), ng::Vector2i(200, 200)) {
  ng::Window* window = new ng::Window(this, "My Window");
  window->setPosition(ng::Vector2i(15, 15));
  window->setLayout(new ng::GroupLayout);

  new ng::Label(window, "My Button", "sans-bold");

  ng::Button* button = new ng::Button(window, "The Button");
  button->setCallback([] {
    const char* cwd = getwd(nullptr);
    std::cout << "Working directory is :" << cwd << "\n";
    std::free((void*)cwd);
  });
  button->setTooltip("Push me!");

  performLayout();
}

void App::drawContents() {
  // TODO: draw visualization?
  table_.draw();
}

bool App::keyboardEvent(int key, int scancode, int action, int modifiers) {
  if (ng::Screen::keyboardEvent(key, scancode, action, modifiers)) {
    return true;
  }
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    setVisible(false);
    return true;
  }
  return false;
}

} // namespace showtime
