#pragma once

#include <array>
#include <cstdint>

#include <vulkan/vulkan_core.h>

namespace engine::vulkan {

constexpr uint32_t MAX_FRAMES_IN_FLIGHT = 2;
constexpr std::array<const char *, 1> validationLayers = {
    "VK_LAYER_KHRONOS_validation"};
constexpr std::array<const char *, 1> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME};

} // namespace engine::vulkan
