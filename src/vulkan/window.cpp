#include "window.hpp"
#include "config.hpp"

#include <stdexcept>

namespace engine::vulkan {

Window::Window(const Config &config) : config(config) {
  if (!glfwInit()) {
    throw std::runtime_error("Failed to initialize GLFW");
  }

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  window = glfwCreateWindow(config.width, config.height, config.title.data(),
                            nullptr, nullptr);

  if (!window) {
    throw std::runtime_error("Failed to create window");
    glfwTerminate();
  }
}

Window::~Window() {
  glfwDestroyWindow(window);
  glfwTerminate();
}

} // namespace engine::vulkan
