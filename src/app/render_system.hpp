#pragma once

#include "../engine/core/lve_object.hpp"
#include "../engine/graphics/vulkan/vk_device.hpp"
#include "../engine/graphics/vulkan/vk_pipeline.hpp"
#include "../engine/renderer/lve_camera.hpp"

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
						std::vector<LVEObject> &objects,
						const LVECamera &camera);

  private:
	void create_vulkan_pipeline_layout();
	void create_vulkan_pipeline(VkRenderPass renderPass);

	LVEVulkanDevice &lveVulkanDevice;

	std::unique_ptr<LVEVulkanPipeline> lveVulkanPipeline;
	VkPipelineLayout vulkanPipelineLayout;
};

} // namespace lve
