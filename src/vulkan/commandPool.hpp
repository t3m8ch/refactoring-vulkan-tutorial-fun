#pragma once

#include "logicalDevice.hpp"
#include <vector>
#include <vulkan/vulkan_core.h>

namespace engine::vulkan {

class CommandPool {
public:
  CommandPool(const LogicalDevice &logicalDevice);
  ~CommandPool();

  CommandPool(const CommandPool &) = delete;
  CommandPool &operator=(const CommandPool &) = delete;
  CommandPool(CommandPool &&) = delete;
  CommandPool &operator=(CommandPool &&) = delete;

private:
  VkCommandPool commandPool;
  std::vector<VkCommandBuffer> commandBuffers;

  const LogicalDevice &logicalDevice;

  void createCommandPool(const LogicalDevice &logicalDevice);
  void createCommandBuffers(const LogicalDevice &logicalDevice);
};

} // namespace engine::vulkan
