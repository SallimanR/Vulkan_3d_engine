#pragma once

#include "core/lve_object.hpp"
#include "graphics/vulkan/vk_device.hpp"
#include "graphics/vulkan/vk_pipeline.hpp"

#include <memory>
#include <vector>

namespace lve {

class RenderSystem {
  public:
	RenderSystem(LVEVulkanDevice &device, VkRenderPass renderPass);
	~RenderSystem();

	RenderSystem(const RenderSystem &) = delete;
	RenderSystem &operator=(const RenderSystem &) = delete;

	void render_objects(VkCommandBuffer commandBuffer,
					   std::vector<LVEObject> &objects);

  private:
	void create_vulkan_pipeline_layout();
	void create_vulkan_pipeline(VkRenderPass renderPass);

	LVEVulkanDevice &lveVulkanDevice;

	std::unique_ptr<LVEVulkanPipeline> lveVulkanPipeline;
	VkPipelineLayout vulkanPipelineLayout;
};

} // namespace lve
