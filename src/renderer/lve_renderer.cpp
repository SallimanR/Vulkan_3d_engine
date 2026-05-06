
#include "../graphics/vulkan/vk_pipeline.hpp"

#include "lve_renderer.hpp"

#include <optional>
#include <vulkan/vulkan_core.h>

#include <array>
#include <memory>
#include <stdexcept>
#include <vector>

namespace lve {

LVERenderer::LVERenderer(LVEWindow &window, LVEVulkanDevice &device)
	: lveWindow{window}, lveVulkanDevice{device} {
	recreateSwapChain();
	createVulkanCommandBuffers();
}

LVERenderer::~LVERenderer() { freeVulkanCommandBuffers(); }

void LVERenderer::recreateSwapChain() {
	auto extent = lveWindow.getExtent();
	while (extent.width == 0 || extent.height == 0) {
		extent = lveWindow.getExtent();
		glfwWaitEvents();
	}
	vkDeviceWaitIdle(lveVulkanDevice.device());

	if (lveVulkanSwapChain == nullptr) {
		lveVulkanSwapChain =
			std::make_unique<LVEVulkanSwapChain>(lveVulkanDevice, extent);
	} else {
		std::shared_ptr<LVEVulkanSwapChain> oldVulkanSwapChain =
			std::move(lveVulkanSwapChain);
		lveVulkanSwapChain = std::make_unique<LVEVulkanSwapChain>(
			lveVulkanDevice, extent, oldVulkanSwapChain);

		if (!oldVulkanSwapChain->compareSwapFormats(
				*lveVulkanSwapChain.get())) {
			// TODO: callback instead of error
			throw std::runtime_error(
				"Swap chain image(or depth) format has changed!");
		}
	}
}

void LVERenderer::createVulkanCommandBuffers() {
	vulkanCommandBuffers.resize(LVEVulkanSwapChain::MAX_FRAMES_IN_FLIGHT);

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

void LVERenderer::freeVulkanCommandBuffers() {
	vkFreeCommandBuffers(lveVulkanDevice.device(),
						 lveVulkanDevice.getCommandPool(),
						 static_cast<uint32_t>(vulkanCommandBuffers.size()),
						 vulkanCommandBuffers.data());
	vulkanCommandBuffers.clear();
}

std::optional<VkCommandBuffer> LVERenderer::beginFrame() {
	assert(!isFrameStarted &&
		   "Can't call beginFrame() while already in progress");

	auto result = lveVulkanSwapChain->acquireNextImage(&currentImageIndex);
	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
		recreateSwapChain();
		return std::nullopt;
	}
	if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		throw std::runtime_error("failed to aquire swapchain image!");
	}

	isFrameStarted = true;

	auto commandBuffer = getCurrentCommandBuffer();
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

	if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
		throw std::runtime_error("failed to begin recording command buffer!");
	};
	return commandBuffer;
}

void LVERenderer::endFrame() {
	assert(isFrameStarted &&
		   "Can't call endFrame() while frame is not in progress");

	auto commandBuffer = getCurrentCommandBuffer();
	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to record command buffer!");
	}

	auto result = lveVulkanSwapChain->submitCommandBuffers(&commandBuffer,
														   &currentImageIndex);
	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
		lveWindow.wasWindowResized()) {
		lveWindow.resetWindowResizedFlag();
		recreateSwapChain();
	} else if (result != VK_SUCCESS) {
		throw std::runtime_error("failed to present swapchain image!");
	}

	isFrameStarted = false;
	currentFrameIndex =
		(currentFrameIndex + 1) % LVEVulkanSwapChain::MAX_FRAMES_IN_FLIGHT;
}

void LVERenderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer) {
	assert(isFrameStarted &&
		   "Can't call beginSwapChainRenderPass() if frame is not in progress");
	assert(commandBuffer == getCurrentCommandBuffer() &&
		   "Can't begin render pass on command buffer from different frame");

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = lveVulkanSwapChain->getRenderPass();
	renderPassInfo.framebuffer =
		lveVulkanSwapChain->getFramebuffer(currentImageIndex);

	renderPassInfo.renderArea.offset = {0, 0};
	renderPassInfo.renderArea.extent = lveVulkanSwapChain->getSwapChainExtent();

	std::array<VkClearValue, 2> clearValues{};
	clearValues[0].color = {0.01f, 0.01f, 0.01f, 1.0f};
	clearValues[1].depthStencil = {1.0f, 0};
	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	renderPassInfo.pClearValues = clearValues.data();

	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo,
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
	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
}

void LVERenderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer) {
	assert(isFrameStarted &&
		   "Can't call endSwapChainRenderPass() if frame is not in progress");
	assert(commandBuffer == getCurrentCommandBuffer() &&
		   "Can't end render pass on command buffer from different frame");

	vkCmdEndRenderPass(commandBuffer);
}

} // namespace lve
