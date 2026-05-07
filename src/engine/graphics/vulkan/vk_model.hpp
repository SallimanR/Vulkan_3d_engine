#pragma once

#include "vk_device.hpp"

#include <vulkan/vulkan_core.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <vector>

namespace lve {

class LVEVulkanModel {
  public:
	struct Vertex {
		glm::vec2 position;
		glm::vec3 color;

		static std::vector<VkVertexInputBindingDescription>
		get_binding_description();
		static std::vector<VkVertexInputAttributeDescription>
		get_attribute_description();
	};

	LVEVulkanModel(LVEVulkanDevice &device,
				   const std::vector<Vertex> &verticies);
	~LVEVulkanModel();

	LVEVulkanModel(const LVEVulkanModel &) = delete;
	LVEVulkanModel &operator=(const LVEVulkanModel &) = delete;

	void bind(VkCommandBuffer commandBuffer);
	void draw(VkCommandBuffer commandBuffer);

  private:
	void create_vertex_buffers(const std::vector<Vertex> &verticies);

	LVEVulkanDevice &vulkanDevice;
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;
	uint32_t vertexCount;
};

} // namespace lve
