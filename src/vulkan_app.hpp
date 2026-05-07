#pragma once

#include "engine/core/lve_object.hpp"
#include "engine/graphics/vulkan/vk_device.hpp"
#include "engine/renderer/lve_renderer.hpp"

#include "engine/platform/lve_window.hpp"

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
	void load_objects();

	LVEWindow lveWindow{WIDTH, HEIGHT, "C++ Vulkan"};
	LVEVulkanDevice lveVulkanDevice{lveWindow};
	LVERenderer lveRenderer{lveWindow, lveVulkanDevice};

	std::vector<LVEObject> objects;
};

} // namespace lve
