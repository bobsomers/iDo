#include "showtime/app.h"

#include <iostream>
#include <cstdlib>
#include <unistd.h>

#include <GLFW/glfw3.h>

#include "showtime/all_off_effect.h"
#include "showtime/all_on_effect.h"
#include "showtime/audio_device.h"
#include "showtime/color_fade_effect.h"
#include "showtime/chase_effect.h"
#include "showtime/cycle_effect.h"
#include "showtime/debug_sink.h"
#include "showtime/network_sink.h"
#include "showtime/test_effect.h"
#include "showtime/audio_effect.h"
#include "showtime/visualizer_sink.h"

namespace showtime {

constexpr int kDebugSink = 0;
constexpr int kNetworkSink = 1;
constexpr int kVisualizerSink = 2;

App::App(GLFWwindow* screen)
  : ng::Screen() {
  initialize(screen, true);

  AudioDevice::initialize();

  // Initial effect is all off.
  setEffect(new AllOffEffect);

  // Set up data sinks.
  sinks_[kDebugSink].reset(new DebugSink);
  sinks_[kNetworkSink].reset(new NetworkSink);
  sinks_[kVisualizerSink].reset(new VisualizerSink);

  // Create UI.
  createControlsWindow();

  // TODO: remove me
  const char* cwd = getwd(nullptr);
  std::cout << "Working directory is :" << cwd << "\n";
  std::free((void*)cwd);

  setVisible(true);
  performLayout();
}

void App::createControlsWindow() {
  ng::Window* w = new ng::Window(this, "Controls");
  w->setPosition(ng::Vector2i(15, 15));
  w->setLayout(new ng::GroupLayout);

  new ng::Label(w, "Active Effect", "sans-bold");

  ng::ComboBox* cb = new ng::ComboBox(w, {
      "       All Off       ",
      "All On",
      "Spectrum",
      "Test Channels"
      "Color Fade",
      "Chase",
      "Cycle Effects",
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
      setEffect(new AudioEffect);
      break;

    case 3:
      setEffect(new TestEffect);
      break;

    case 4:
      setEffect(new ColorFadeEffect(0.5));
      break;

    case 4:
      setEffect(new ChaseEffect(0.2));
      break;

    case 5:
      setEffect(new CycleEffect(10.0, 20.0));
      break;

    default:
      setEffect(new AllOffEffect);
    }
  });

  new ng::Label(w, "Output Sink", "sans-bold");

  ng::CheckBox* box = new ng::CheckBox(w, "Debug");
  box->setCallback([this](bool checked) {
    sinks_[kDebugSink]->setActive(checked);
  });

  box = new ng::CheckBox(w, "Network");
  box->setCallback([this](bool checked) {
    sinks_[kNetworkSink]->setActive(checked);
  });

  box = new ng::CheckBox(w, "Visualizer");
  box->setCallback([this](bool checked) {
    sinks_[kVisualizerSink]->setActive(checked);
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

  glClearColor(0.2f, 0.25f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  for (auto& sink : sinks_) {
    if (sink->active()) {
      sink->sink(c);
    }
  }
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
