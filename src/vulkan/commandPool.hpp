#pragma once

#include "logicalDevice.hpp"
#include "pipeline.hpp"
#include "renderPass.hpp"
#include "swapChain.hpp"

#include <vector>
#include <vulkan/vulkan_core.h>

namespace engine::vulkan {

class CommandPool {
public:
  CommandPool(const LogicalDevice &logicalDevice, const RenderPass &renderPass,
              const SwapChain &swapChain, const Pipeline &pipeline);
  ~CommandPool();

  CommandPool(const CommandPool &) = delete;
  CommandPool &operator=(const CommandPool &) = delete;
  CommandPool(CommandPool &&) = delete;
  CommandPool &operator=(CommandPool &&) = delete;

  VkCommandBuffer getCommandBuffer(uint32_t commandBufferIndex) {
    return commandBuffers[commandBufferIndex];
  }
  void resetCommandBuffer(uint32_t commandBufferIndex);
  void recordCommandBuffer(uint32_t commandBufferIndex, uint32_t imageIndex);

private:
  VkCommandPool commandPool;
  std::vector<VkCommandBuffer> commandBuffers;

  const LogicalDevice &logicalDevice;
  const RenderPass &renderPass;
  const SwapChain &swapChain;
  const Pipeline &pipeline;

  void createCommandPool(const LogicalDevice &logicalDevice);
  void createCommandBuffers(const LogicalDevice &logicalDevice);
};

} // namespace engine::vulkan
