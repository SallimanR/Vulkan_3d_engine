#include "vulkan_app.hpp"

#include "render_system.hpp"

#include <utility>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <vulkan/vulkan_core.h>

#include <memory>
#include <vector>

namespace lve {

LVEVulkanApp::LVEVulkanApp() { load_objects(); }

LVEVulkanApp::~LVEVulkanApp() {}

void LVEVulkanApp::run() {
	RenderSystem renderSystem{lveVulkanDevice,
							  lveRenderer.get_swapchain_render_pass()};

	while (!lveWindow.should_close()) {
		glfwPollEvents();

		if (auto commandBuffer = lveRenderer.begin_frame()) {
			lveRenderer.begin_swap_chain_render_pass(*commandBuffer);
			renderSystem.render_objects(*commandBuffer, objects);
			lveRenderer.end_swap_chain_render_pass(*commandBuffer);
			lveRenderer.end_frame();
		}
	}
}

void LVEVulkanApp::load_objects() {
	std::vector<LVEVulkanModel::Vertex> verticies = {
		{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
		{{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
		{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}};

	auto lveModel =
		std::make_shared<LVEVulkanModel>(lveVulkanDevice, verticies);

	auto triangle = LVEObject::create_object();
	triangle.model = lveModel;
	triangle.color = {.1f, .8f, .1f};
	triangle.transform2d.translation.x = .2f;
	triangle.transform2d.scale = {2.f, .5f};
	triangle.transform2d.rotation = .25f * glm::two_pi<float>();

	objects.push_back(std::move(triangle));
}

} // namespace lve
