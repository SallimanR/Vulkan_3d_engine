#pragma once

#include "lve_window.hpp"
#include "src/graphics/vulkan/vk_pipeline.hpp"

namespace lve {

class LVEAppBase {
public:
	static constexpr int WIDTH = 800;
	static constexpr int HEIGHT = 600;

	void run();

private:
	LVEWindow lveWindow{ WIDTH, HEIGHT, "C++ Vulkan" };
	LVEVulkanPipeline lveVulkanPipeline{ "assets/spirv/triangle.vert.spv", "assets/spirv/triangle.frag.spv" };
};

} //namespace lve
