#pragma once

#include "core/lve_object.hpp"
#include "graphics/vulkan/vk_device.hpp"
#include "renderer/lve_renderer.hpp"

#include "platform/lve_window.hpp"

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

	LVEWindow lveWindow{WIDTH, HEIGHT, "C++ Vulkan"};
	LVEVulkanDevice lveVulkanDevice{lveWindow};
	LVERenderer lveRenderer{lveWindow, lveVulkanDevice};

	std::vector<LVEObject> objects;
};

} // namespace lve
