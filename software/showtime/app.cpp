#include "showtime/app.h"

#include <iostream>
#include <cstdlib>
#include <unistd.h>

#include <GLFW/glfw3.h>

#include "showtime/all_off_effect.h"
#include "showtime/all_on_effect.h"
#include "showtime/debug_sink.h"
#include "showtime/test_effect.h"

namespace showtime {

App::App(GLFWwindow* screen)
  : ng::Screen() {
  initialize(screen, true);

  // Initial effect is all off.
  setEffect(new AllOffEffect);

  // Set up data sinks.
  sinks_.emplace_back(new DebugSink);

  // Create UI.
  createEffectsWindow();

  // TODO: remove me
  const char* cwd = getwd(nullptr);
  std::cout << "Working directory is :" << cwd << "\n";
  std::free((void*)cwd);

  setVisible(true);
  performLayout();
}

void App::createEffectsWindow() {
  ng::Window* w = new ng::Window(this, "Controls");
  w->setPosition(ng::Vector2i(15, 15));
  w->setLayout(new ng::GroupLayout);

  new ng::Label(w, "Active Effect", "sans-bold");

  ng::ComboBox* cb = new ng::ComboBox(w, {
      "       All Off       ",
      "All On",
      "Test Channels"
  });
  cb->setCallback([this](int selected) {
    switch (selected) {
    case 0:
      setEffect(new AllOffEffect);
      break;

    case 1:
      setEffect(new AllOnEffect);
      break;

    case 2:
      setEffect(new TestEffect);
      break;

    default:
      setEffect(new AllOffEffect);
    }
  });
}

void App::setEffect(Effect* new_effect) {
  effect_ = EffectPtr(new_effect);
  effect_t_start_ = glfwGetTime();
}

void App::drawContents() {
  // TODO: move this background thread?
  ColorChannels c;
  if (effect_) {
    c = effect_->process(glfwGetTime() - effect_t_start_, nullptr);
  }
  for (auto& sink : sinks_) {
    sink->sink(c);
  }

  glClearColor(0.2f, 0.25f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  //const float aspect_ratio = static_cast<float>(mSize.y()) / mSize.x();

  //visualizer_->draw(aspect_ratio);
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
