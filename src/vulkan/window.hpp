#pragma once

#define GLFW_INCLUDE_VULKAN

#include "config.hpp"

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

namespace engine::vulkan {
class Window {
public:
  Window(const Config &config);
  ~Window();

  Window(const Window &) = delete;
  Window &operator=(const Window &) = delete;
  Window(Window &&) = delete;
  Window &operator=(Window &&) = delete;

  GLFWwindow *data() const { return window; };

private:
  GLFWwindow *window;
  const Config &config;
};
} // namespace engine::vulkan
