#include "app.hpp"

namespace engine::vulkan {
VulkanApp::VulkanApp(const Config &config) : config(config) {}

void VulkanApp::run() {
  while (!glfwWindowShouldClose(window.data())) {
    glfwPollEvents();
  }
}
} // namespace engine::vulkan
