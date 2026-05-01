#pragma once

#include "graphics/vulkan/vk_device.hpp"
#include "src/graphics/vulkan/vk_pipeline.hpp"
#include "src/graphics/vulkan/vk_swapchain.hpp"

#include "platform/lve_window.hpp"

#include <memory>
#include <vector>

namespace lve {

class LVEAppBase {
public:
	static constexpr int WIDTH = 800;
	static constexpr int HEIGHT = 600;

	LVEAppBase();
	~LVEAppBase();

	LVEAppBase(const LVEAppBase &) = delete;
	LVEAppBase &operator=(const LVEAppBase &) = delete;

	void run();

private:
	void createVulkanPipelineLayout();
	void createVulkanPipeline();
	void createVulkanCommandBuffers();
	void drawFrame();

	LVEWindow lveWindow{ WIDTH, HEIGHT, "C++ Vulkan" };
	LVEVulkanDevice lveVulkanDevice{ lveWindow };
	LVEVulkanSwapChain lveVulkanSwapChain{ lveVulkanDevice, lveWindow.getExtent() };
	std::unique_ptr<LVEVulkanPipeline> lveVulkanPipeline;

	VkPipelineLayout vulkanPipelineLayout;
	std::vector<VkCommandBuffer> vulkanCommandBuffers;
};

} //namespace lve
