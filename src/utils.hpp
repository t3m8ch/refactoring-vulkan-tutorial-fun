#pragma once

#include <cstddef>
#include <fstream>
#include <vector>

namespace engine::utils {

template <typename T> auto enumerate(const T &container) {
  size_t i = 0;
  std::vector<std::tuple<size_t, typename T::const_reference>> result;
  for (const auto &element : container) {
    result.emplace_back(i++, element);
  }
  return result;
}

static std::vector<char> readFile(const std::string &filename) {
  std::ifstream file(filename, std::ios::ate | std::ios::binary);

  if (!file.is_open()) {
    throw std::runtime_error("failed to open file!");
  }

  size_t fileSize = (size_t)file.tellg();
  std::vector<char> buffer(fileSize);

  file.seekg(0);
  file.read(buffer.data(), fileSize);
  file.close();

  return buffer;
}

} // namespace engine::utils
