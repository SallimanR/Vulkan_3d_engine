#include "render_system.hpp"

#include "graphics/vulkan/vk_pipeline.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <vulkan/vulkan_core.h>

#include <memory>
#include <stdexcept>
#include <vector>

namespace lve {

struct VulkanPushConstantData {
	// Identity matrix, every value on main (left-up corner) diagonal is 1
	glm::mat2 transform{1.f};

	glm::vec2 offset;
	alignas(16) glm::vec3 color;
};

RenderSystem::RenderSystem(LVEVulkanDevice &device, VkRenderPass renderPass)
	: lveVulkanDevice{device} {
	create_vulkan_pipeline_layout();
	create_vulkan_pipeline(renderPass);
}

RenderSystem::~RenderSystem() {
	vkDestroyPipelineLayout(lveVulkanDevice.device(), vulkanPipelineLayout,
							nullptr);
}

void RenderSystem::create_vulkan_pipeline_layout() {
	VkPushConstantRange pushConstantRange{};
	pushConstantRange.stageFlags =
		VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
	pushConstantRange.offset = 0;
	pushConstantRange.size = sizeof(VulkanPushConstantData);

	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 0;
	pipelineLayoutInfo.pSetLayouts = nullptr;
	pipelineLayoutInfo.pushConstantRangeCount = 1;
	pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
	if (vkCreatePipelineLayout(lveVulkanDevice.device(), &pipelineLayoutInfo,
							   nullptr, &vulkanPipelineLayout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create pipeline layout");
	}
}

void RenderSystem::create_vulkan_pipeline(VkRenderPass renderPass) {
	assert(vulkanPipelineLayout != nullptr &&
		   "Cannot create pipeline before pipeline layout");

	VulkanPipelineConfigInfo pipelineConfig{};
	LVEVulkanPipeline::default_vulkan_pipeline_config_info(pipelineConfig);
	pipelineConfig.renderPass = renderPass;
	pipelineConfig.pipelineLayout = vulkanPipelineLayout;
	lveVulkanPipeline = std::make_unique<LVEVulkanPipeline>(
		lveVulkanDevice, "assets/spirv/triangle.vert.spv",
		"assets/spirv/triangle.frag.spv", pipelineConfig);
}

void RenderSystem::render_objects(VkCommandBuffer commandBuffer,
								 std::vector<LVEObject> &objects) {
	lveVulkanPipeline->bind(commandBuffer);

	for (auto &obj : objects) {
		VulkanPushConstantData push{};
		push.offset = obj.transform2d.translation;
		push.color = obj.color;
		push.transform = obj.transform2d.mat2();

		vkCmdPushConstants(commandBuffer, vulkanPipelineLayout,
						   VK_SHADER_STAGE_VERTEX_BIT |
							   VK_SHADER_STAGE_FRAGMENT_BIT,
						   0, sizeof(VulkanPushConstantData), &push);
		obj.model->bind(commandBuffer);
		obj.model->draw(commandBuffer);
	}
}

} // namespace lve
