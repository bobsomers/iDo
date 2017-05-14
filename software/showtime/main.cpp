#include <cstdlib>
#include <iostream>

#if defined(NANOGUI_GLAD)
# if defined(NANOGUI_SHARED) && !defined(GLAD_GLAPI_EXPORT)
#   define GLAD_GLAPI_EXPORT
# endif
#   include <glad/glad.h>
#else
# if defined(__APPLE__)
#   define GLFW_INCLUDE_GLCOREARB
# else
#   define GL_GLEXT_PROTOTYPES
# endif
#endif

#include <GLFW/glfw3.h>
#include <nanogui/nanogui.h>

#include "showtime/app.h"

namespace ng = nanogui;
namespace st = showtime;

st::App* app = nullptr;

int main() {
  glfwSetErrorCallback([](int error, const char* description) {
    std::cerr << "GLFW Error: [" << error << "] " << description << "\n";
  });

  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW.\n";
    return EXIT_FAILURE;
  }

  glfwSetTime(0.0);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  glfwWindowHint(GLFW_SAMPLES, 0);
  glfwWindowHint(GLFW_RED_BITS, 8);
  glfwWindowHint(GLFW_GREEN_BITS, 8);
  glfwWindowHint(GLFW_BLUE_BITS, 8);
  glfwWindowHint(GLFW_ALPHA_BITS, 8);
  glfwWindowHint(GLFW_STENCIL_BITS, 8);
  glfwWindowHint(GLFW_DEPTH_BITS, 24);
  glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

  GLFWwindow* window = glfwCreateWindow(1280, 600, "Showtime", nullptr, nullptr);
  if (!window) {
      std::cerr << "Failed to create GLFW window.\n";
      glfwTerminate();
      return EXIT_FAILURE;
  }
  glfwMakeContextCurrent(window);

#if defined(NANOGUI_GLAD)
  if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
    std::cerr << "Could not initialize GLAD!\n";
    glfwTerminate();
    return EXIT_FAILURE;
  }
  glGetError();
#endif

  glClearColor(0.2f, 0.25f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  app = new st::App(window);

  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  glViewport(0, 0, width, height);
  glfwSwapInterval(0);
  glfwSwapBuffers(window);

  glfwSetCursorPosCallback(window, [](GLFWwindow*, double x, double y) {
    app->cursorPosCallbackEvent(x, y);
  });

  glfwSetMouseButtonCallback(window, [](GLFWwindow*, int button, int action, int modifiers) {
    app->mouseButtonCallbackEvent(button, action, modifiers);
  });

  glfwSetKeyCallback(window, [](GLFWwindow*, int key, int scancode, int action, int mods) {
    app->keyCallbackEvent(key, scancode, action, mods);
  });

  glfwSetCharCallback(window, [](GLFWwindow*, unsigned int codepoint) {
    app->charCallbackEvent(codepoint);
  });

  glfwSetDropCallback(window, [](GLFWwindow*, int count, const char **filenames) {
    app->dropCallbackEvent(count, filenames);
  });

  glfwSetScrollCallback(window, [](GLFWwindow*, double x, double y) {
    app->scrollCallbackEvent(x, y);
  });

  glfwSetFramebufferSizeCallback(window, [](GLFWwindow*, int width, int height) {
    app->resizeCallbackEvent(width, height);
  });

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    app->drawContents();
    app->drawWidgets();
    glfwSwapBuffers(window);
  }

  delete app;
  glfwTerminate();

  return EXIT_SUCCESS;
}
