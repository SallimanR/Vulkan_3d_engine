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

	VkRenderPass get_swapchain_render_pass() const {
		return lveVulkanSwapChain->get_render_pass();
	}
	float get_aspect_ratio() const {
		return lveVulkanSwapChain->extent_aspect_ratio();
	}
	bool is_frame_in_progress() const { return isFrameStarted; }

	std::optional<VkCommandBuffer> begin_frame();
	void end_frame();
	void begin_swap_chain_render_pass(VkCommandBuffer commandBuffer);
	void end_swap_chain_render_pass(VkCommandBuffer commandBuffer);

	VkCommandBuffer get_current_command_buffer() const {
		assert(isFrameStarted &&
			   "Cannot get command buffer when frame not in progress");
		return vulkanCommandBuffers[currentFrameIndex];
	}

	int get_frame_index() const {
		assert(isFrameStarted &&
			   "Cannot get frame index when frame not in progress");

		return currentFrameIndex;
	}

  private:
	void create_vulkan_command_buffers();
	void free_vulkan_command_buffers();
	void recreate_swap_chain();

	LVEWindow &lveWindow;

	LVEVulkanDevice &lveVulkanDevice;
	std::unique_ptr<LVEVulkanSwapChain> lveVulkanSwapChain;
	std::vector<VkCommandBuffer> vulkanCommandBuffers;

	uint32_t currentImageIndex;
	int currentFrameIndex{0};
	bool isFrameStarted = false;
};

} // namespace lve
