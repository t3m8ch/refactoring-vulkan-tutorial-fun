#pragma once

#include "config.hpp"
#include "instance.hpp"
#include "logicalDevice.hpp"
#include "physicalDevice.hpp"
#include "pipeline.hpp"
#include "renderPass.hpp"
#include "surface.hpp"
#include "swapChain.hpp"
#include "sync.hpp"
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
  RenderPass renderPass{logicalDevice};
  SwapChain swapChain{logicalDevice, window, surface, renderPass, config};
  Pipeline pipeline{logicalDevice, swapChain, renderPass, config};
  Sync sync{logicalDevice};
};
} // namespace engine::vulkan
