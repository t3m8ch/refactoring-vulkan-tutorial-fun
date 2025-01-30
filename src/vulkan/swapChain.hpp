#pragma once

#include "logicalDevice.hpp"
#include "renderPass.hpp"
#include "window.hpp"

#include <vulkan/vulkan_core.h>

namespace engine::vulkan {

class SwapChain {
public:
  SwapChain(const LogicalDevice &logicalDevice, const Window &window,
            const Surface &surface, const RenderPass &renderPass,
            const Config &config);
  ~SwapChain();

  SwapChain(const SwapChain &) = delete;
  SwapChain &operator=(const SwapChain &) = delete;
  SwapChain(SwapChain &&) = delete;
  SwapChain &operator=(SwapChain &&) = delete;

  VkSwapchainKHR data() const { return swapChain; }
  VkExtent2D getExtent() const { return swapChainExtent; }

private:
  VkSwapchainKHR swapChain;
  std::vector<VkImage> swapChainImages;
  std::vector<VkImageView> swapChainImageViews;
  VkFormat swapChainImageFormat;
  VkExtent2D swapChainExtent;
  std::vector<VkFramebuffer> swapChainFramebuffers;

  const LogicalDevice &logicalDevice;
  const Config &config;

  VkPresentModeKHR chooseSwapPresentMode(
      const std::vector<VkPresentModeKHR> &availablePresentModes);
  VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities,
                              GLFWwindow *window);

  void createSwapChain(const LogicalDevice &logicalDevice, const Window &window,
                       const Surface &surface, const Config &config);
  void createImageViews(const LogicalDevice &logicalDevice);
  void createFramebuffers(const LogicalDevice &logicalDevice,
                          const RenderPass &renderPass);
};

} // namespace engine::vulkan
