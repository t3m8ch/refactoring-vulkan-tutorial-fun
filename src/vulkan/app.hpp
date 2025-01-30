#pragma once

#include "config.hpp"
#include "instance.hpp"
#include "logicalDevice.hpp"
#include "physicalDevice.hpp"
#include "surface.hpp"
#include "window.hpp"

namespace engine::vulkan {
class VulkanApp {
public:
  VulkanApp(const Config &config);

  void run();

private:
  const Config &config;
  Window window{config};
  Instance instance{config};
  Surface surface{instance, window};
  PhysicalDevice physicalDevice{instance, surface};
  LogicalDevice logicalDevice{physicalDevice};
};
} // namespace engine::vulkan
