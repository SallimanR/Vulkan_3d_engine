#include "vulkan_app.hpp"

#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>

#include <array>
#include <memory>
#include <stdexcept>
#include <vector>

namespace lve {

LVEVulkanApp::LVEVulkanApp() {
	loadModels();
	createVulkanPipelineLayout();
	createVulkanPipeline();
	createVulkanCommandBuffers();
}

LVEVulkanApp::~LVEVulkanApp() {
	vkDestroyPipelineLayout(lveVulkanDevice.device(), vulkanPipelineLayout, nullptr);
}

void LVEVulkanApp::run() {
	while (!lveWindow.shouldClose()) {
		glfwPollEvents();
		drawFrame();
	}
}

void LVEVulkanApp::loadModels() {
	std::vector<LVEVulkanModel::Vertex> verticies = {
		{ { 0.0f, -0.5f } },
		{ { 0.5f, 0.5f } },
		{ { -0.5f, 0.5f } }
	};

	lveModel = std::make_unique<LVEVulkanModel>(lveVulkanDevice, verticies);
}

void LVEVulkanApp::createVulkanPipelineLayout() {
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

void LVEVulkanApp::createVulkanPipeline() {
	auto pipelineConfig = LVEVulkanPipeline::defaultVulkanPipelineConfigInfo(lveVulkanSwapChain.width(), lveVulkanSwapChain.height());
	pipelineConfig.renderPass = lveVulkanSwapChain.getRenderPass();
	pipelineConfig.pipelineLayout = vulkanPipelineLayout;
	lveVulkanPipeline = std::make_unique<LVEVulkanPipeline>(
			lveVulkanDevice,
			"assets/spirv/triangle.vert.spv",
			"assets/spirv/triangle.frag.spv",
			pipelineConfig);
}

void LVEVulkanApp::createVulkanCommandBuffers() {
	vulkanCommandBuffers.resize(lveVulkanSwapChain.imageCount());

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = lveVulkanDevice.getCommandPool();
	allocInfo.commandBufferCount = static_cast<uint32_t>(vulkanCommandBuffers.size());

	if (vkAllocateCommandBuffers(lveVulkanDevice.device(), &allocInfo, vulkanCommandBuffers.data()) !=
			VK_SUCCESS) {
		throw std::runtime_error("failed to allocate command buffer!");
	}

	for (int i = 0; i < vulkanCommandBuffers.size(); i++) {
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(vulkanCommandBuffers[i], &beginInfo) != VK_SUCCESS) {
			throw std::runtime_error("failed to begin recording command buffer!");
		};

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = lveVulkanSwapChain.getRenderPass();
		renderPassInfo.framebuffer = lveVulkanSwapChain.getFramebuffer(i);

		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = lveVulkanSwapChain.getSwapChainExtent();

		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = { 0.1f, 0.1f, 0.1f, 1.0f };
		clearValues[1].depthStencil = { 1.0f, 0 };
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(vulkanCommandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		lveVulkanPipeline->bind(vulkanCommandBuffers[i]);
		lveModel->bind(vulkanCommandBuffers[i]);
		lveModel->draw(vulkanCommandBuffers[i]);

		vkCmdEndRenderPass(vulkanCommandBuffers[i]);
		if (vkEndCommandBuffer(vulkanCommandBuffers[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to record command buffer!");
		}
	}
}

void LVEVulkanApp::drawFrame() {
	uint32_t imageIndex;
	auto result = lveVulkanSwapChain.acquireNextImage(&imageIndex);

	if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		throw std::runtime_error("failed to aquire swapchain image!");
	}

	result = lveVulkanSwapChain.submitCommandBuffers(&vulkanCommandBuffers[imageIndex], &imageIndex);
	if (result != VK_SUCCESS) {
		throw std::runtime_error("failed to present swapchain image!");
	}
}

} //namespace lve
