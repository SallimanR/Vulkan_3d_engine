#include "lve_app_base.hpp"

#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>

#include <memory>
#include <stdexcept>

namespace lve {

LVEAppBase::LVEAppBase() {
	createVulkanPipelineLayout();
	createVulkanPipeline();
	createVulkanCommandBuffers();
}

LVEAppBase::~LVEAppBase() {
	vkDestroyPipelineLayout(lveVulkanDevice.device(), vulkanPipelineLayout, nullptr);
}

void LVEAppBase::run() {
	while (!lveWindow.shouldClose()) {
		glfwPollEvents();
	}
}

void LVEAppBase::createVulkanPipelineLayout() {
	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 0;
	pipelineLayoutInfo.pSetLayouts = nullptr;
	pipelineLayoutInfo.pushConstantRangeCount = 0;
	pipelineLayoutInfo.pPushConstantRanges = nullptr;
	if (vkCreatePipelineLayout(lveVulkanDevice.device(), &pipelineLayoutInfo, nullptr, &vulkanPipelineLayout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create pipeline layout");
	}
}
void LVEAppBase::createVulkanPipeline() {
	auto pipelineConfig = LVEVulkanPipeline::defaultVulkanPipelineConfigInfo(lveVulkanSwapChain.width(), lveVulkanSwapChain.height());
	pipelineConfig.renderPass = lveVulkanSwapChain.getRenderPass();
	pipelineConfig.pipelineLayout = vulkanPipelineLayout;
	lveVulkanPipeline = std::make_unique<LVEVulkanPipeline>(
			lveVulkanDevice,
			"assets/spirv/triangle.vert.spv",
			"assets/spirv/triangle.frag.spv",
			pipelineConfig);
}

void LVEAppBase::createVulkanCommandBuffers() {
}

void LVEAppBase::drawFrame() {
}

} //namespace lve
