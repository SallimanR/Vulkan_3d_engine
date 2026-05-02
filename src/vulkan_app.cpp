#include "vulkan_app.hpp"

#include "src/graphics/vulkan/vk_pipeline.hpp"

#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <vulkan/vulkan_core.h>

#include <array>
#include <memory>
#include <stdexcept>
#include <vector>

namespace lve {

struct VulkanPushConstantData {
	glm::vec2 offset;
	alignas(16) glm::vec3 color;
};

LVEVulkanApp::LVEVulkanApp() {
	loadModels();
	createVulkanPipelineLayout();
	recreateSwapChain();
	createVulkanCommandBuffers();
}

LVEVulkanApp::~LVEVulkanApp() {
	vkDestroyPipelineLayout(lveVulkanDevice.device(), vulkanPipelineLayout,
							nullptr);
}

void LVEVulkanApp::run() {
	while (!lveWindow.shouldClose()) {
		glfwPollEvents();
		drawFrame();
	}
}

void LVEVulkanApp::loadModels() {
	std::vector<LVEVulkanModel::Vertex> verticies = {
		{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
		{{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
		{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}};

	lveModel = std::make_unique<LVEVulkanModel>(lveVulkanDevice, verticies);
}

void LVEVulkanApp::createVulkanPipelineLayout() {
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

void LVEVulkanApp::recreateSwapChain() {
	auto extent = lveWindow.getExtent();
	while (extent.width == 0 || extent.height == 0) {
		extent = lveWindow.getExtent();
		glfwWaitEvents();
	}
	vkDeviceWaitIdle(lveVulkanDevice.device());

	// TODO: replace this
	lveVulkanSwapChain = nullptr;

	if (lveVulkanSwapChain == nullptr) {
		lveVulkanSwapChain =
			std::make_unique<LVEVulkanSwapChain>(lveVulkanDevice, extent);
	} else {
		lveVulkanSwapChain = std::make_unique<LVEVulkanSwapChain>(
			lveVulkanDevice, extent, std::move(lveVulkanSwapChain));
		if (lveVulkanSwapChain->imageCount() != vulkanCommandBuffers.size()) {
			freeVulkanCommandBuffers();
			createVulkanCommandBuffers();
		}
	}

	createVulkanPipeline();
}

void LVEVulkanApp::createVulkanPipeline() {
	assert(lveVulkanSwapChain != nullptr &&
		   "Cannot create pipeline before swap chain");
	assert(vulkanPipelineLayout != nullptr &&
		   "Cannot create pipeline before pipeline layout");

	VulkanPipelineConfigInfo pipelineConfig{};
	LVEVulkanPipeline::defaultVulkanPipelineConfigInfo(pipelineConfig);
	pipelineConfig.renderPass = lveVulkanSwapChain->getRenderPass();
	pipelineConfig.pipelineLayout = vulkanPipelineLayout;
	lveVulkanPipeline = std::make_unique<LVEVulkanPipeline>(
		lveVulkanDevice, "assets/spirv/triangle.vert.spv",
		"assets/spirv/triangle.frag.spv", pipelineConfig);
}

void LVEVulkanApp::createVulkanCommandBuffers() {
	vulkanCommandBuffers.resize(lveVulkanSwapChain->imageCount());

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = lveVulkanDevice.getCommandPool();
	allocInfo.commandBufferCount =
		static_cast<uint32_t>(vulkanCommandBuffers.size());

	if (vkAllocateCommandBuffers(lveVulkanDevice.device(), &allocInfo,
								 vulkanCommandBuffers.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate command buffer!");
	}
}

void LVEVulkanApp::freeVulkanCommandBuffers() {
	vkFreeCommandBuffers(lveVulkanDevice.device(),
						 lveVulkanDevice.getCommandPool(),
						 static_cast<uint32_t>(vulkanCommandBuffers.size()),
						 vulkanCommandBuffers.data());
	vulkanCommandBuffers.clear();
}

void LVEVulkanApp::recordCommandBuffer(int imageIndex) {
	static int frame = 0;
	frame = (frame + 1) % 1000;

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

	if (vkBeginCommandBuffer(vulkanCommandBuffers[imageIndex], &beginInfo) !=
		VK_SUCCESS) {
		throw std::runtime_error("failed to begin recording command buffer!");
	};

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = lveVulkanSwapChain->getRenderPass();
	renderPassInfo.framebuffer = lveVulkanSwapChain->getFramebuffer(imageIndex);

	renderPassInfo.renderArea.offset = {0, 0};
	renderPassInfo.renderArea.extent = lveVulkanSwapChain->getSwapChainExtent();

	std::array<VkClearValue, 2> clearValues{};
	clearValues[0].color = {0.1f, 0.1f, 0.1f, 1.0f};
	clearValues[1].depthStencil = {1.0f, 0};
	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	renderPassInfo.pClearValues = clearValues.data();

	vkCmdBeginRenderPass(vulkanCommandBuffers[imageIndex], &renderPassInfo,
						 VK_SUBPASS_CONTENTS_INLINE);

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width =
		static_cast<float>(lveVulkanSwapChain->getSwapChainExtent().width);
	viewport.height =
		static_cast<float>(lveVulkanSwapChain->getSwapChainExtent().height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	VkRect2D scissor{{0, 0}, lveVulkanSwapChain->getSwapChainExtent()};
	vkCmdSetViewport(vulkanCommandBuffers[imageIndex], 0, 1, &viewport);
	vkCmdSetScissor(vulkanCommandBuffers[imageIndex], 0, 1, &scissor);

	lveVulkanPipeline->bind(vulkanCommandBuffers[imageIndex]);
	lveModel->bind(vulkanCommandBuffers[imageIndex]);

	for (int j = 0; j < 4; j++) {
		VulkanPushConstantData push{};
		push.offset = {0.0f, -0.4f + j * 0.25f};
		push.color = {0.0f, 0.0f, 0.2f + 0.2f * j};

		vkCmdPushConstants(
			vulkanCommandBuffers[imageIndex], vulkanPipelineLayout,
			VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0,
			sizeof(VulkanPushConstantData), &push);

		lveModel->draw(vulkanCommandBuffers[imageIndex]);
	}

	vkCmdEndRenderPass(vulkanCommandBuffers[imageIndex]);
	if (vkEndCommandBuffer(vulkanCommandBuffers[imageIndex]) != VK_SUCCESS) {
		throw std::runtime_error("failed to record command buffer!");
	}
}

void LVEVulkanApp::drawFrame() {
	uint32_t imageIndex;
	auto result = lveVulkanSwapChain->acquireNextImage(&imageIndex);
	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
		recreateSwapChain();
		return;
	}
	if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		throw std::runtime_error("failed to aquire swapchain image!");
	}

	recordCommandBuffer(imageIndex);
	result = lveVulkanSwapChain->submitCommandBuffers(
		&vulkanCommandBuffers[imageIndex], &imageIndex);
	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
		lveWindow.wasWindowResized()) {
		lveWindow.resetWindowResizedFlag();
		recreateSwapChain();
		return;
	} else if (result != VK_SUCCESS) {
		throw std::runtime_error("failed to present swapchain image!");
	}
}

} // namespace lve
