#pragma once

#include "vk_device.hpp"

#include <vulkan/vulkan.h>

#include <vector>

namespace lve {

class LVEVulkanSwapChain {
public:
	static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

	LVEVulkanSwapChain(LVEVulkanDevice &deviceRef, VkExtent2D windowExtent);
	~LVEVulkanSwapChain();

	LVEVulkanSwapChain(const LVEVulkanSwapChain &) = delete;
	LVEVulkanSwapChain operator=(const LVEVulkanSwapChain &) = delete;

	VkFramebuffer getFramebuffer(int index) { return swapChainFramebuffers[index]; }
	VkRenderPass getRenderPass() { return renderPass; }
	VkImageView getImageView(int index) { return swapChainImageViews[index]; }
	size_t imageCount() { return swapChainImageViews.size(); }
	VkFormat getSwapChainImageFormat() { return swapChainImageFormat; }
	VkExtent2D getSwapChainExtent() { return swapChainExtent; }
	uint32_t width() { return swapChainExtent.width; }
	uint32_t height() { return swapChainExtent.height; }

	float extentAspectRatio() {
		return static_cast<float>(swapChainExtent.width) / static_cast<float>(swapChainExtent.height);
	}
	VkFormat findDepthFormat();

	VkResult acquireNextImage(uint32_t *imageIndex);
	VkResult submitCommandBuffers(const VkCommandBuffer *buffers, uint32_t *imageIndex);

private:
	void createSwapChain();
	void createImageViews();
	void createDepthResources();
	void createRenderPass();
	void createFramebuffers();
	void createSyncObjects();

	// Helper functions
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(
			const std::vector<VkSurfaceFormatKHR> &availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(
			const std::vector<VkPresentModeKHR> &availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);

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
	VkExtent2D swapChainExtent;

	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;
	std::vector<VkFence> imagesInFlight;
	size_t currentFrame = 0;
};

} //namespace lve
