#pragma once

#define GLFW_INCLUDE_VULKAN

#include "config.hpp"

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

namespace engine::vulkan {
class Window {
public:
  struct Size {
    uint32_t width;
    uint32_t height;
  };

  Window(const Config &config);
  ~Window();

  Window(const Window &) = delete;
  Window &operator=(const Window &) = delete;
  Window(Window &&) = delete;
  Window &operator=(Window &&) = delete;

  bool resized = false;

  GLFWwindow *data() const { return window; };
  Size getSize() const;
  void waitEvents() const;

private:
  GLFWwindow *window;
  const Config &config;

  static void framebufferResizeCallback(GLFWwindow *window, int width,
                                        int height);
};
} // namespace engine::vulkan
