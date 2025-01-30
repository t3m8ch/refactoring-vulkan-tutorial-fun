#define GLFW_INCLUDE_VULKAN

#include "surface.hpp"
#include <GLFW/glfw3.h>
#include <stdexcept>

namespace engine::vulkan {

Surface::Surface(const Instance &instance, const Window &window)
    : instance(instance) {
  if (glfwCreateWindowSurface(instance.data(), window.data(), nullptr,
                              &surface) != VK_SUCCESS) {
    throw std::runtime_error("failed to create window surface!");
  }
}

Surface::~Surface() { vkDestroySurfaceKHR(instance.data(), surface, nullptr); }

} // namespace engine::vulkan
