#pragma once

#include "core/lve_object.hpp"
#include "graphics/vulkan/vk_device.hpp"
#include "graphics/vulkan/vk_pipeline.hpp"
#include "renderer/lve_renderer.hpp"

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
	void renderObjects(VkCommandBuffer commandBuffer);

	LVEWindow lveWindow{WIDTH, HEIGHT, "C++ Vulkan"};
	LVEVulkanDevice lveVulkanDevice{lveWindow};
	LVERenderer lveRenderer{lveWindow, lveVulkanDevice};

	std::unique_ptr<LVEVulkanPipeline> lveVulkanPipeline;
	VkPipelineLayout vulkanPipelineLayout;

	std::vector<LVEObject> objects;
};

} // namespace lve
