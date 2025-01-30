#pragma once

#include "config.hpp"
#include "instance.hpp"
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
};
} // namespace engine::vulkan
