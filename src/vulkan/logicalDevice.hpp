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

private:
  VkDevice device;
  VkQueue graphicsQueue;
  VkQueue presentQueue;
};

} // namespace engine::vulkan
