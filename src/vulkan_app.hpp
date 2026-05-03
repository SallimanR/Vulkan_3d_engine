#pragma once

#include "core/lve_object.hpp"
#include "graphics/vulkan/vk_device.hpp"
#include "src/graphics/vulkan/vk_pipeline.hpp"
#include "src/graphics/vulkan/vk_swapchain.hpp"

#include "platform/lve_window.hpp"

#include <memory>
#include <vector>

namespace lve {

class LVEVulkanApp {
  public:
	static constexpr int WIDTH = 800;
	static constexpr int HEIGHT = 600;

	LVEVulkanApp();
	~LVEVulkanApp();

	LVEVulkanApp(const LVEVulkanApp &) = delete;
	LVEVulkanApp &operator=(const LVEVulkanApp &) = delete;

	void run();

  private:
	void loadObjects();
	void createVulkanPipelineLayout();
	void createVulkanPipeline();
	void createVulkanCommandBuffers();
	void freeVulkanCommandBuffers();
	void drawFrame();
	void recreateSwapChain();
	void recordCommandBuffer(int imageIndex);
	void renderObjects(VkCommandBuffer commandBuffer);

	LVEWindow lveWindow{WIDTH, HEIGHT, "C++ Vulkan"};
	LVEVulkanDevice lveVulkanDevice{lveWindow};
	std::unique_ptr<LVEVulkanSwapChain> lveVulkanSwapChain;
	std::unique_ptr<LVEVulkanPipeline> lveVulkanPipeline;

	VkPipelineLayout vulkanPipelineLayout;
	std::vector<VkCommandBuffer> vulkanCommandBuffers;
	std::vector<LVEObject> objects;
};

} // namespace lve
