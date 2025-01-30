#pragma once

#include "instance.hpp"
#include "surface.hpp"

#include <optional>

#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_core.h>

namespace engine::vulkan {

class PhysicalDevice {
public:
  struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() {
      return graphicsFamily.has_value() && presentFamily.has_value();
    }
  };

  struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
  };

  PhysicalDevice(const Instance &instance, const Surface &surface);

  PhysicalDevice(const PhysicalDevice &) = delete;
  PhysicalDevice &operator=(const PhysicalDevice &) = delete;
  PhysicalDevice(PhysicalDevice &&) = delete;
  PhysicalDevice &operator=(PhysicalDevice &&) = delete;

  VkPhysicalDevice data() const { return physicalDevice; };

  QueueFamilyIndices getQueueFamilyIndices() const {
    return queueFamilyIndices;
  }

  SwapChainSupportDetails getSwapChainSupport() const {
    return swapChainSupport;
  }

private:
  VkPhysicalDevice physicalDevice;
  QueueFamilyIndices queueFamilyIndices;
  SwapChainSupportDetails swapChainSupport;

  int32_t rateDeviceSuitability(VkPhysicalDevice device, VkSurfaceKHR surface);
  bool checkDeviceExtensionSupport(VkPhysicalDevice device);
  QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device,
                                       VkSurfaceKHR surface);
  SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device,
                                                VkSurfaceKHR surface);
};

} // namespace engine::vulkan
