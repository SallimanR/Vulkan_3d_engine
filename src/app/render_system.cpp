#include "render_system.hpp"
#include <glm/common.hpp>

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
	glm::mat4 transform{1.f};
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
		lveVulkanDevice, "assets/spirv/cube.vert.spv",
		"assets/spirv/cube.frag.spv", pipelineConfig);
}

void RenderSystem::render_objects(VkCommandBuffer commandBuffer,
								  std::vector<LVEObject> &objects,
								  const LVECamera &camera) {
	lveVulkanPipeline->bind(commandBuffer);

	for (auto &obj : objects) {
		obj.transform.rotation.y =
			glm::mod(obj.transform.rotation.y + 0.01f, glm::two_pi<float>());
		obj.transform.rotation.x =
			glm::mod(obj.transform.rotation.x + 0.005f, glm::two_pi<float>());
		obj.transform.rotation.z =
			glm::mod(obj.transform.rotation.z + 0.005f, glm::two_pi<float>());

		VulkanPushConstantData push{};
		push.color = obj.color;
		push.transform = camera.get_projection() * obj.transform.mat4();

		vkCmdPushConstants(commandBuffer, vulkanPipelineLayout,
						   VK_SHADER_STAGE_VERTEX_BIT |
							   VK_SHADER_STAGE_FRAGMENT_BIT,
						   0, sizeof(VulkanPushConstantData), &push);
		obj.model->bind(commandBuffer);
		obj.model->draw(commandBuffer);
	}
}

} // namespace lve
