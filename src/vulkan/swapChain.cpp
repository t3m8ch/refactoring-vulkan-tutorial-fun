#include "swapChain.hpp"
#include "logicalDevice.hpp"
#include "window.hpp"

#include <array>
#include <iostream>

#include <limits>
#include <vulkan/vulkan_core.h>

namespace engine::vulkan {

SwapChain::SwapChain(const LogicalDevice &logicalDevice, const Window &window,
                     const Surface &surface, const RenderPass &renderPass,
                     const Config &config)
    : config(config), logicalDevice(logicalDevice) {
  createSwapChain(logicalDevice, window, surface, config);
  createImageViews(logicalDevice);
  createFramebuffers(logicalDevice, renderPass);
}

SwapChain::~SwapChain() {
  for (const auto &framebuffer : swapChainFramebuffers) {
    vkDestroyFramebuffer(logicalDevice.data(), framebuffer, nullptr);
  }

  for (const auto &imageView : swapChainImageViews) {
    vkDestroyImageView(logicalDevice.data(), imageView, nullptr);
  }

  vkDestroySwapchainKHR(logicalDevice.data(), swapChain, nullptr);
}

void SwapChain::createSwapChain(const LogicalDevice &logicalDevice,
                                const Window &window, const Surface &surface,
                                const Config &config) {
  auto swapChainSupport =
      logicalDevice.getPhysicalDevice().getSwapChainSupport();
  auto surfaceFormat =
      logicalDevice.getPhysicalDevice().getSwapChainSupport().chosenFormat;
  auto presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
  auto extent = chooseSwapExtent(swapChainSupport.capabilities, window.data());

  uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
  if (swapChainSupport.capabilities.maxImageCount > 0 &&
      imageCount > swapChainSupport.capabilities.maxImageCount) {
    imageCount = swapChainSupport.capabilities.maxImageCount;
  }

  VkSwapchainCreateInfoKHR createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  createInfo.surface = surface.data();
  createInfo.minImageCount = imageCount;
  createInfo.imageFormat = surfaceFormat.format;
  createInfo.imageColorSpace = surfaceFormat.colorSpace;
  createInfo.imageExtent = extent;
  createInfo.imageArrayLayers = 1;
  createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

  auto indices = logicalDevice.getPhysicalDevice().getQueueFamilyIndices();
  std::array<uint32_t, 2> queueFamilyIndices = {indices.graphicsFamily.value(),
                                                indices.presentFamily.value()};

  if (indices.graphicsFamily != indices.presentFamily) {
    createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    createInfo.queueFamilyIndexCount = 2;
    createInfo.pQueueFamilyIndices = queueFamilyIndices.data();
  } else {
    createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    createInfo.queueFamilyIndexCount = 0;     // Optional
    createInfo.pQueueFamilyIndices = nullptr; // Optional
  }

  createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
  createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  createInfo.presentMode = presentMode;
  createInfo.clipped = VK_TRUE;
  createInfo.oldSwapchain = VK_NULL_HANDLE;

  if (vkCreateSwapchainKHR(logicalDevice.data(), &createInfo, nullptr,
                           &swapChain) != VK_SUCCESS) {
    throw std::runtime_error("failed to create swap chain!");
  }

  vkGetSwapchainImagesKHR(logicalDevice.data(), swapChain, &imageCount,
                          nullptr);
  swapChainImages.resize(imageCount);
  vkGetSwapchainImagesKHR(logicalDevice.data(), swapChain, &imageCount,
                          swapChainImages.data());

  swapChainImageFormat = surfaceFormat.format;
  swapChainExtent = extent;
}

void SwapChain::createImageViews(const LogicalDevice &logicalDevice) {
  swapChainImageViews.resize(swapChainImages.size());
  for (size_t i = 0; i < swapChainImages.size(); i++) {
    VkImageViewCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    createInfo.image = swapChainImages[i];
    createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    createInfo.format = swapChainImageFormat;

    createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

    createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    createInfo.subresourceRange.baseMipLevel = 0;
    createInfo.subresourceRange.levelCount = 1;
    createInfo.subresourceRange.baseArrayLayer = 0;
    createInfo.subresourceRange.layerCount = 1;

    if (vkCreateImageView(logicalDevice.data(), &createInfo, nullptr,
                          &swapChainImageViews[i]) != VK_SUCCESS) {
      throw std::runtime_error("failed to create image views!");
    }
  }
}

void SwapChain::createFramebuffers(const LogicalDevice &logicalDevice,
                                   const RenderPass &renderPass) {
  swapChainFramebuffers.resize(swapChainImageViews.size());

  for (size_t i = 0; i < swapChainImageViews.size(); i++) {
    std::array<VkImageView, 1> attachments = {swapChainImageViews[i]};

    VkFramebufferCreateInfo framebufferInfo{};
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass = renderPass.data();
    framebufferInfo.attachmentCount = 1;
    framebufferInfo.pAttachments = attachments.data();
    framebufferInfo.width = swapChainExtent.width;
    framebufferInfo.height = swapChainExtent.height;
    framebufferInfo.layers = 1;

    if (vkCreateFramebuffer(logicalDevice.data(), &framebufferInfo, nullptr,
                            &swapChainFramebuffers[i]) != VK_SUCCESS) {
      throw std::runtime_error("failed to create framebuffer!");
    }
  }
}

VkPresentModeKHR SwapChain::chooseSwapPresentMode(
    const std::vector<VkPresentModeKHR> &availablePresentModes) {

  for (const auto &availablePresentMode : availablePresentModes) {
    if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
      return availablePresentMode;
    }
  }

  VkPresentModeKHR vkChosenPresentMode;
  if (config.presentMode == FIFO) {
    vkChosenPresentMode = VK_PRESENT_MODE_FIFO_KHR;
  } else if (config.presentMode == MAILBOX) {
    vkChosenPresentMode = VK_PRESENT_MODE_MAILBOX_KHR;
  } else if (config.presentMode == IMMEDIATE) {
    vkChosenPresentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
  }

  for (const auto &availablePresentMode : availablePresentModes) {
    if (availablePresentMode == vkChosenPresentMode) {
      std::cout << "Chosen present mode is available!" << std::endl;
      return vkChosenPresentMode;
    }
  }

  std::cerr << "Chosen present mode is not available! FIFO will be used"
            << std::endl;
  return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D
SwapChain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities,
                            GLFWwindow *window) {
  if (capabilities.currentExtent.width !=
      std::numeric_limits<uint32_t>::max()) {
    return capabilities.currentExtent;
  } else {
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    VkExtent2D actualExtent = {static_cast<uint32_t>(width),
                               static_cast<uint32_t>(height)};

    actualExtent.width =
        std::clamp(actualExtent.width, capabilities.minImageExtent.width,
                   capabilities.maxImageExtent.width);
    actualExtent.height =
        std::clamp(actualExtent.height, capabilities.minImageExtent.height,
                   capabilities.maxImageExtent.height);

    return actualExtent;
  }
}

} // namespace engine::vulkan
