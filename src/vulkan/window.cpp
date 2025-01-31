#include "window.hpp"
#include "config.hpp"

#include <stdexcept>

namespace engine::vulkan {

Window::Window(const Config &config) : config(config) {
  if (!glfwInit()) {
    throw std::runtime_error("Failed to initialize GLFW");
  }

  glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

  window = glfwCreateWindow(config.width, config.height, config.title.data(),
                            nullptr, nullptr);

  if (!window) {
    throw std::runtime_error("Failed to create window");
    glfwTerminate();
  }

  glfwSetWindowUserPointer(window, this);
}

Window::~Window() {
  glfwDestroyWindow(window);
  glfwTerminate();
}

void Window::framebufferResizeCallback(GLFWwindow *window, int width,
                                       int height) {
  auto engineWindow =
      reinterpret_cast<Window *>(glfwGetWindowUserPointer(window));
  engineWindow->resized = true;
}

Window::Size Window::getSize() const {
  int width, height;
  glfwGetWindowSize(window, &width, &height);
  return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};
}

void Window::waitEvents() const { glfwWaitEvents(); }

} // namespace engine::vulkan
