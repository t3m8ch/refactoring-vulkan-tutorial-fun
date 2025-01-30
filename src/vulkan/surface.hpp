#pragma once

#include "instance.hpp"
#include "window.hpp"

#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_core.h>

namespace engine::vulkan {

class Instance;

class Surface {
public:
  Surface(const Instance &instance, const Window &window);
  ~Surface();

  Surface(const Surface &) = delete;
  Surface &operator=(const Surface &) = delete;
  Surface(Surface &&) = delete;
  Surface &operator=(Surface &&) = delete;

  VkSurfaceKHR get() const { return surface; };

private:
  VkSurfaceKHR surface;
  const Instance &instance;
};

} // namespace engine::vulkan
