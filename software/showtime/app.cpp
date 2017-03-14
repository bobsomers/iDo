#include "showtime/app.h"

#include <iostream>
#include <cstdlib>
#include <unistd.h>

#include "showtime/types.h"

namespace showtime {

App::App(GLFWwindow* screen)
  : ng::Screen() {
  initialize(screen, true);

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

  visualizer_.reset(new Visualizer);
  visualizer_->addTable(Vec2f(0.5f, 0.5f), Vec2f(0.5f, 0.5f));

  setVisible(true);
  performLayout();
}

void App::drawContents() {
  glClearColor(0.2f, 0.25f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  const float aspect_ratio = static_cast<float>(mSize.y()) / mSize.x();

  visualizer_->draw(aspect_ratio);
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
