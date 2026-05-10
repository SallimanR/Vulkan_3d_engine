#pragma once

#include "vk_device.hpp"

#include <vulkan/vulkan.h>

#include <memory>
#include <vector>

namespace lve {

class LVEVulkanSwapChain {
  public:
	static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

	LVEVulkanSwapChain(LVEVulkanDevice &deviceRef, VkExtent2D windowExtent);
	LVEVulkanSwapChain(LVEVulkanDevice &deviceRef, VkExtent2D windowExtent,
					   std::shared_ptr<LVEVulkanSwapChain> previousSwapchain);
	~LVEVulkanSwapChain();

	LVEVulkanSwapChain(const LVEVulkanSwapChain &) = delete;
	LVEVulkanSwapChain operator=(const LVEVulkanSwapChain &) = delete;

	VkFramebuffer get_framebuffer(int index) {
		return swapChainFramebuffers[index];
	}
	VkRenderPass get_render_pass() { return renderPass; }
	VkImageView get_image_view(int index) { return swapChainImageViews[index]; }
	size_t image_count() { return swapChainImageViews.size(); }
	VkFormat get_swap_chain_image_format() { return swapChainImageFormat; }
	VkExtent2D get_swap_chain_extent() { return swapChainExtent; }
	uint32_t width() { return swapChainExtent.width; }
	uint32_t height() { return swapChainExtent.height; }

	float extent_aspect_ratio() {
		return static_cast<float>(swapChainExtent.width) /
			   static_cast<float>(swapChainExtent.height);
	}
	VkFormat find_depth_format();

	VkResult acquire_next_image(uint32_t *imageIndex);
	VkResult submit_command_buffers(const VkCommandBuffer *buffers,
									uint32_t *imageIndex);

	bool compare_swap_formats(const LVEVulkanSwapChain &swapChain) const {
		return swapChain.swapChainDepthFormat == swapChainDepthFormat &&
			   swapChain.swapChainImageFormat == swapChainImageFormat;
	};

  private:
	void init();

	void create_swap_chain();
	void create_image_views();
	void create_depth_resources();
	void create_render_pass();
	void create_framebuffers();
	void create_sync_objects();

	// Helper functions
	VkSurfaceFormatKHR choose_swap_surface_format(
		const std::vector<VkSurfaceFormatKHR> &availableFormats);
	VkPresentModeKHR choose_swap_present_mode(
		const std::vector<VkPresentModeKHR> &availablePresentModes);
	VkExtent2D choose_swap_extent(const VkSurfaceCapabilitiesKHR &capabilities);

	std::vector<VkFramebuffer> swapChainFramebuffers;
	VkRenderPass renderPass;

	std::vector<VkImage> depthImages;
	std::vector<VkDeviceMemory> depthImageMemorys;
	std::vector<VkImageView> depthImageViews;
	std::vector<VkImage> swapChainImages;
	std::vector<VkImageView> swapChainImageViews;

	LVEVulkanDevice &vulkanDevice;
	VkExtent2D windowExtent;

	VkSwapchainKHR swapChain;
	VkFormat swapChainImageFormat;
	VkFormat swapChainDepthFormat;
	VkExtent2D swapChainExtent;
	std::shared_ptr<LVEVulkanSwapChain> oldSwapChain;

	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;
	std::vector<VkFence> imagesInFlight;
	size_t currentFrame = 0;
};

} // namespace lve
