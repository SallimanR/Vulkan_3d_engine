#pragma once

#include "src/graphics/vulkan/vk_device.hpp"

#include <vulkan/vulkan_core.h>

#include <cstdint>
#include <string>
#include <vector>

namespace lve {

struct VulkanPipelineConfigInfo {
	VulkanPipelineConfigInfo() = default;
	VulkanPipelineConfigInfo(const VulkanPipelineConfigInfo &) = delete;
	VulkanPipelineConfigInfo &operator=(const VulkanPipelineConfigInfo &) = delete;

	VkPipelineViewportStateCreateInfo viewportInfo;
	VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
	VkPipelineRasterizationStateCreateInfo rasterizationInfo;
	VkPipelineMultisampleStateCreateInfo multisampleInfo;
	VkPipelineColorBlendAttachmentState colorBlendAttachment;
	VkPipelineColorBlendStateCreateInfo colorBlendInfo;
	VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
	std::vector<VkDynamicState> dynamicStateEnables;
	VkPipelineDynamicStateCreateInfo dynamicStateInfo;
	VkPipelineLayout pipelineLayout = nullptr;
	VkRenderPass renderPass = nullptr;
	uint32_t subpass = 0;
};

class LVEVulkanPipeline {
public:
	LVEVulkanPipeline(
			LVEVulkanDevice &device,
			const std::string &vertFilePath,
			const std::string &fragFilePath,
			const VulkanPipelineConfigInfo &configInfo);
	~LVEVulkanPipeline();

	LVEVulkanPipeline(const LVEVulkanPipeline &) = delete;
	LVEVulkanPipeline operator=(const LVEVulkanPipeline &) = delete;

	void bind(VkCommandBuffer commandBuffer);
	static void defaultVulkanPipelineConfigInfo(VulkanPipelineConfigInfo &configInfo);

private:
	static std::vector<char> readFile(const std::string &filePath);

	void createGraphicsPipeline(
			const std::string &vertFilePath,
			const std::string &fragFilePath,
			const VulkanPipelineConfigInfo &configInfo);

	void createShaderModule(const std::vector<char> &code, VkShaderModule *shaderModule);

	LVEVulkanDevice &vulkanDevice;
	VkPipeline graphicsPipeline;
	VkShaderModule vertShaderModule;
	VkShaderModule fragShaderModule;
};

} //namespace lve
