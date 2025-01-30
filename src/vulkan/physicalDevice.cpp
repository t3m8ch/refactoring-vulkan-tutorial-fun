#include "physicalDevice.hpp"
#include "../utils.hpp"
#include "constants.hpp"
#include "instance.hpp"
#include "surface.hpp"

#include <cstring>
#include <map>
#include <set>
#include <stdexcept>

#include <vulkan/vulkan_core.h>

namespace engine::vulkan {

PhysicalDevice::PhysicalDevice(const Instance &instance,
                               const Surface &surface) {
  uint32_t deviceCount = 0;
  vkEnumeratePhysicalDevices(instance.data(), &deviceCount, nullptr);

  if (deviceCount == 0) {
    throw std::runtime_error("failed to find GPUs with Vulkan support!");
  }

  std::vector<VkPhysicalDevice> devices(deviceCount);
  vkEnumeratePhysicalDevices(instance.data(), &deviceCount, devices.data());

  std::multimap<int32_t, VkPhysicalDevice> candidates;

  for (const auto &device : devices) {
    int score = rateDeviceSuitability(device, surface.data());
    candidates.insert(std::make_pair(score, device));
  }

  if (candidates.rbegin()->first > 0) {
    physicalDevice = candidates.rbegin()->second;

    swapChainSupport = querySwapChainSupport(physicalDevice, surface.data());
    queueFamilyIndices = findQueueFamilies(physicalDevice, surface.data());
  } else {
    throw std::runtime_error("Failed to find a suitable GPU!");
  }

  if (physicalDevice == VK_NULL_HANDLE) {
    throw std::runtime_error("failed to find a suitable GPU!");
  }
}

int32_t PhysicalDevice::rateDeviceSuitability(VkPhysicalDevice device,
                                              VkSurfaceKHR surface) {
  VkPhysicalDeviceProperties properties;
  vkGetPhysicalDeviceProperties(device, &properties);

  VkPhysicalDeviceFeatures features;
  vkGetPhysicalDeviceFeatures(device, &features);

  int32_t score = 0;

  if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
    score += 1000;
  }

  if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU) {
    score += 500;
  }

  if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU) {
    score += 100;
  }

  score += properties.limits.maxImageDimension2D;

  bool extensionsSupported = checkDeviceExtensionSupport(device);

  bool swapChainAdequate = false;
  if (extensionsSupported) {
    auto swapChainSupport = querySwapChainSupport(device, surface);
    swapChainAdequate = !swapChainSupport.formats.empty() &&
                        !swapChainSupport.presentModes.empty();
  }

  auto queueFamilyIndices = findQueueFamilies(device, surface);
  if (!features.geometryShader || !queueFamilyIndices.isComplete() ||
      !extensionsSupported || !swapChainAdequate) {
    return 0;
  }

  return score;
}

bool PhysicalDevice::checkDeviceExtensionSupport(VkPhysicalDevice device) {
  uint32_t extensionCount;
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount,
                                       nullptr);

  std::vector<VkExtensionProperties> availableExtensions(extensionCount);
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount,
                                       availableExtensions.data());

  std::set<std::string> requiredExtensions(deviceExtensions.begin(),
                                           deviceExtensions.end());

  for (const auto &extension : availableExtensions) {
    requiredExtensions.erase(extension.extensionName);
  }

  return requiredExtensions.empty();
}

PhysicalDevice::QueueFamilyIndices
PhysicalDevice::findQueueFamilies(VkPhysicalDevice device,
                                  VkSurfaceKHR surface) {
  PhysicalDevice::QueueFamilyIndices indices;

  uint32_t queueFamilyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

  std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount,
                                           queueFamilies.data());

  for (const auto &[i, queueFamily] : engine::utils::enumerate(queueFamilies)) {
    if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
      indices.graphicsFamily = i;
    }

    VkBool32 presentSupport = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

    if (presentSupport) {
      indices.presentFamily = i;
    }

    if (indices.isComplete()) {
      break;
    }
  }

  return indices;
}

PhysicalDevice::SwapChainSupportDetails
PhysicalDevice::querySwapChainSupport(VkPhysicalDevice device,
                                      VkSurfaceKHR surface) {
  SwapChainSupportDetails details;

  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface,
                                            &details.capabilities);

  uint32_t formatCount;
  vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

  if (formatCount != 0) {
    details.formats.resize(formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount,
                                         details.formats.data());
  }

  uint32_t presentModeCount;
  vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount,
                                            nullptr);

  if (presentModeCount != 0) {
    details.presentModes.resize(presentModeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(
        device, surface, &presentModeCount, details.presentModes.data());
  }

  return details;
}

} // namespace engine::vulkan
