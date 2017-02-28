#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include "nanogui/nanogui.h"
#include "showtime/app.h"

namespace ng = nanogui;
namespace st = showtime;

int main() {
  try {
    ng::init();
    {
      ng::ref<st::App> app = new st::App();
      app->drawAll();
      app->setVisible(true);
      ng::mainloop();
    }
    ng::shutdown();
  } catch(const std::runtime_error& e) {
    std::cerr << "Runtime Error: " << e.what() << "\n";
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
