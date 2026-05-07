#include "vk_model.hpp"

#include <vulkan/vulkan_core.h>

#include <cassert>
#include <cstddef>
#include <cstring>
#include <vector>

namespace lve {

LVEVulkanModel::LVEVulkanModel(LVEVulkanDevice &device,
							   const std::vector<Vertex> &verticies)
	: vulkanDevice{device} {
	create_vertex_buffers(verticies);
}

LVEVulkanModel::~LVEVulkanModel() {
	vkDestroyBuffer(vulkanDevice.device(), vertexBuffer, nullptr);
	vkFreeMemory(vulkanDevice.device(), vertexBufferMemory, nullptr);
}

void LVEVulkanModel::create_vertex_buffers(const std::vector<Vertex> &verticies) {
	vertexCount = static_cast<uint32_t>(verticies.size());
	assert(vertexCount >= 3 && "Vertex count must be at least 3");
	VkDeviceSize vertexBufferSize = sizeof(verticies[0]) * vertexCount;
	vulkanDevice.create_buffer(vertexBufferSize,
							  VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
							  VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
								  VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
							  vertexBuffer, vertexBufferMemory);

	void *data;
	vkMapMemory(vulkanDevice.device(), vertexBufferMemory, 0, vertexBufferSize,
				0, &data);
	memcpy(data, verticies.data(), static_cast<size_t>(vertexBufferSize));
	vkUnmapMemory(vulkanDevice.device(), vertexBufferMemory);
}

void LVEVulkanModel::draw(VkCommandBuffer commandBuffer) {
	vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
}

void LVEVulkanModel::bind(VkCommandBuffer commandBuffer) {
	VkBuffer buffers[] = {vertexBuffer};
	VkDeviceSize offsets[] = {0};
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
}

std::vector<VkVertexInputBindingDescription>
LVEVulkanModel::Vertex::get_binding_description() {
	std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
	bindingDescriptions[0].binding = 0;
	bindingDescriptions[0].stride = sizeof(Vertex);
	bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
	return bindingDescriptions;
}

std::vector<VkVertexInputAttributeDescription>
LVEVulkanModel::Vertex::get_attribute_description() {
	std::vector<VkVertexInputAttributeDescription> attributeDescriptions(2);
	attributeDescriptions[0].binding = 0;
	attributeDescriptions[0].location = 0;
	attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
	attributeDescriptions[0].offset = offsetof(Vertex, position);

	attributeDescriptions[1].binding = 0;
	attributeDescriptions[1].location = 1;
	attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[1].offset = offsetof(Vertex, color);
	return attributeDescriptions;
}

} // namespace lve
