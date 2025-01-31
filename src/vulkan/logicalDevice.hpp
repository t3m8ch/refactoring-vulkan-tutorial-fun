#pragma once

#include "physicalDevice.hpp"
#include <vulkan/vulkan_core.h>

namespace engine::vulkan {

class LogicalDevice {
public:
  LogicalDevice(const PhysicalDevice &physicalDevice);
  ~LogicalDevice();

  LogicalDevice(const LogicalDevice &) = delete;
  LogicalDevice &operator=(const LogicalDevice &) = delete;
  LogicalDevice(LogicalDevice &&) = delete;
  LogicalDevice &operator=(LogicalDevice &&) = delete;

  VkDevice data() const { return device; }
  VkQueue getGraphicsQueue() const { return graphicsQueue; }
  VkQueue getPresentQueue() const { return presentQueue; }
  const PhysicalDevice &getPhysicalDevice() const { return physicalDevice; }

  void graphicsQueueSubmit(VkCommandBuffer commandBuffer,
                           VkSemaphore waitSemaphore,
                           VkSemaphore signalSemaphore,
                           VkFence inFlightFence) const;
  void waitIdle() const;

private:
  VkDevice device;
  VkQueue graphicsQueue;
  VkQueue presentQueue;
  const PhysicalDevice &physicalDevice;
};

} // namespace engine::vulkan
