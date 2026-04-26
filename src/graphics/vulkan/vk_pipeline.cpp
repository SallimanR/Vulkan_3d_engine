#include "vk_pipeline.hpp"

#include <fstream>
#include <iostream>
#include <stdexcept>

namespace lve {

LVEVulkanPipeline::LVEVulkanPipeline(const std::string &vertFilePath, const std::string &fragFilePath) {
	LVEVulkanPipeline::createGraphicsPipeline(vertFilePath, fragFilePath);
}

std::vector<char> LVEVulkanPipeline::readFile(const std::string &filePath) {
	std::ifstream file{ filePath, std::ios::ate | std::ios::binary };

	if (!file.is_open()) {
		throw std::runtime_error("failed to open file path: " + filePath);
	}

	size_t fileSize = static_cast<size_t>(file.tellg());
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);
	return buffer;
}

void LVEVulkanPipeline::createGraphicsPipeline(const std::string &vertFilePath, const std::string &fragFilePath) {
	auto verCode = readFile(vertFilePath);
	auto fragCode = readFile(fragFilePath);

	std::cout << "Vertex Shader Code Size: " << verCode.size() << '\n';
	std::cout << "Fragment Shader Code Size: " << fragCode.size() << '\n';
}

} //namespace lve
