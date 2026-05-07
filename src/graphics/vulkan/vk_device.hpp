#pragma once

#include "../../platform/lve_window.hpp"

#include <vector>

namespace lve {

struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

struct QueueFamilyIndices {
	uint32_t graphicsFamily;
	uint32_t presentFamily;
	bool graphicsFamilyHasValue = false;
	bool presentFamilyHasValue = false;
	bool is_complete() {
		return graphicsFamilyHasValue && presentFamilyHasValue;
	}
};

class LVEVulkanDevice {
  public:
#ifdef NDEBUG
	const bool enableValidationLayers = false;
#else
	const bool enableValidationLayers = true;
#endif
	LVEVulkanDevice(LVEWindow &window);
	~LVEVulkanDevice();

	// Not copyable or movable
	LVEVulkanDevice(const LVEVulkanDevice &) = delete;
	LVEVulkanDevice operator=(const LVEVulkanDevice &) = delete;
	LVEVulkanDevice(LVEVulkanDevice &&) = delete;
	LVEVulkanDevice &operator=(LVEVulkanDevice &&) = delete;

	VkCommandPool get_command_pool() { return commandPool; }
	VkDevice device() { return device_; }
	VkSurfaceKHR surface() { return surface_; }
	VkQueue graphics_queue() { return graphicsQueue_; }
	VkQueue present_queue() { return presentQueue_; }

	SwapChainSupportDetails get_swap_chain_support() {
		return query_swap_chain_support(physicalDevice);
	}
	uint32_t find_memory_type(uint32_t typeFilter,
							VkMemoryPropertyFlags properties);
	QueueFamilyIndices find_physical_queue_families() {
		return find_queue_families(physicalDevice);
	}
	VkFormat find_supported_format(const std::vector<VkFormat> &candidates,
								 VkImageTiling tiling,
								 VkFormatFeatureFlags features);

	// Buffer Helper Functions
	void create_buffer(VkDeviceSize size, VkBufferUsageFlags usage,
					  VkMemoryPropertyFlags properties, VkBuffer &buffer,
					  VkDeviceMemory &bufferMemory);
	VkCommandBuffer begin_single_time_commands();
	void end_single_time_commands(VkCommandBuffer commandBuffer);
	void copy_buffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
	void copy_buffer_to_image(VkBuffer buffer, VkImage image, uint32_t width,
						   uint32_t height, uint32_t layerCount);

	void create_image_with_info(const VkImageCreateInfo &imageInfo,
							 VkMemoryPropertyFlags properties, VkImage &image,
							 VkDeviceMemory &imageMemory);

	VkPhysicalDeviceProperties properties;

  private:
	void create_instance();
	void setup_debug_messenger();
	void create_surface();
	void pick_physical_device();
	void create_logical_device();
	void create_command_pool();

	// helper functions
	bool is_device_suitable(VkPhysicalDevice device);
	std::vector<const char *> get_required_extensions();
	bool check_validation_layer_support();
	QueueFamilyIndices find_queue_families(VkPhysicalDevice device);
	void populate_debug_messenger_create_info(
		VkDebugUtilsMessengerCreateInfoEXT &createInfo);
	void has_gflw_required_instance_extensions();
	bool check_device_extension_support(VkPhysicalDevice device);
	SwapChainSupportDetails query_swap_chain_support(VkPhysicalDevice device);

	VkInstance instance;
	VkDebugUtilsMessengerEXT debugMessenger;
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	LVEWindow &window;
	VkCommandPool commandPool;

	VkDevice device_;
	VkSurfaceKHR surface_;
	VkQueue graphicsQueue_;
	VkQueue presentQueue_;

	const std::vector<const char *> validationLayers = {
		"VK_LAYER_KHRONOS_validation"};
	const std::vector<const char *> deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME};
};

} // namespace lve
