#include "commandPool.hpp"
#include "constants.hpp"
#include "logicalDevice.hpp"

namespace engine::vulkan {

CommandPool::CommandPool(const LogicalDevice &logicalDevice)
    : logicalDevice(logicalDevice) {
  createCommandPool(logicalDevice);
  createCommandBuffers(logicalDevice);
}

CommandPool::~CommandPool() {
  vkDestroyCommandPool(logicalDevice.data(), commandPool, nullptr);
}

void CommandPool::createCommandPool(const LogicalDevice &logicalDevice) {
  auto queueFamilyIndices =
      logicalDevice.getPhysicalDevice().getQueueFamilyIndices();

  VkCommandPoolCreateInfo poolInfo{};
  poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
  poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

  if (vkCreateCommandPool(logicalDevice.data(), &poolInfo, nullptr,
                          &commandPool) != VK_SUCCESS) {
    throw std::runtime_error("failed to create command pool!");
  }
}

void CommandPool::createCommandBuffers(const LogicalDevice &logicalDevice) {
  commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

  VkCommandBufferAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.commandPool = commandPool;
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

  if (vkAllocateCommandBuffers(logicalDevice.data(), &allocInfo,
                               commandBuffers.data()) != VK_SUCCESS) {
    throw std::runtime_error("failed to allocate command buffers!");
  }
}

} // namespace engine::vulkan
