#include <iostream>

#include "vulkan/app.hpp"

int main() {
  engine::vulkan::Config config;

  config.title = "Refactoring Vulkan Tutorial";
  config.width = 800;
  config.height = 600;

  config.appName = "Refactoring Vulkan Tutorial";
  config.appVersion = {0, 0, 1};
  config.engineName = "Refactoring Vulkan Tutorial";
  config.engineVersion = {0, 0, 1};
  config.enableValidationLayers = true;

  try {
    engine::vulkan::VulkanApp app{config};
    app.run();
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
