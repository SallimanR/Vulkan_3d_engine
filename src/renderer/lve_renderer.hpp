#pragma once

#include "../graphics/vulkan/vk_device.hpp"
#include "../graphics/vulkan/vk_swapchain.hpp"

#include "../platform/lve_window.hpp"

#include <cassert>
#include <memory>
#include <optional>
#include <vector>

namespace lve {

class LVERenderer {
  public:
	LVERenderer(LVEWindow &window, LVEVulkanDevice &device);
	~LVERenderer();

	LVERenderer(const LVERenderer &) = delete;
	LVERenderer &operator=(const LVERenderer &) = delete;

	VkRenderPass getSwapchainRenderPass() const {
		return lveVulkanSwapChain->getRenderPass();
	}
	bool isFrameInProgress() const { return isFrameStarted; }

	std::optional<VkCommandBuffer> beginFrame();
	void endFrame();
	void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
	void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

	VkCommandBuffer getCurrentCommandBuffer() const {
		assert(isFrameStarted &&
			   "Cannot get command buffer when frame not in progress");
		return vulkanCommandBuffers[currentImageIndex];
	}

  private:
	void createVulkanCommandBuffers();
	void freeVulkanCommandBuffers();
	void recreateSwapChain();

	LVEWindow &lveWindow;

	LVEVulkanDevice &lveVulkanDevice;
	std::unique_ptr<LVEVulkanSwapChain> lveVulkanSwapChain;
	std::vector<VkCommandBuffer> vulkanCommandBuffers;

	uint32_t currentImageIndex;
	bool isFrameStarted = false;
};

} // namespace lve
