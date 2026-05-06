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

LVEVulkanApp::LVEVulkanApp() { loadObjects(); }

LVEVulkanApp::~LVEVulkanApp() {}

void LVEVulkanApp::run() {
	RenderSystem renderSystem{lveVulkanDevice,
							  lveRenderer.getSwapchainRenderPass()};

	while (!lveWindow.shouldClose()) {
		glfwPollEvents();

		if (auto commandBuffer = lveRenderer.beginFrame()) {
			lveRenderer.beginSwapChainRenderPass(*commandBuffer);
			renderSystem.renderObjects(*commandBuffer, objects);
			lveRenderer.endSwapChainRenderPass(*commandBuffer);
			lveRenderer.endFrame();
		}
	}
}

void LVEVulkanApp::loadObjects() {
	std::vector<LVEVulkanModel::Vertex> verticies = {
		{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
		{{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
		{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}};

	auto lveModel =
		std::make_shared<LVEVulkanModel>(lveVulkanDevice, verticies);

	auto triangle = LVEObject::createObject();
	triangle.model = lveModel;
	triangle.color = {.1f, .8f, .1f};
	triangle.transform2d.translation.x = .2f;
	triangle.transform2d.scale = {2.f, .5f};
	triangle.transform2d.rotation = .25f * glm::two_pi<float>();

	objects.push_back(std::move(triangle));
}

} // namespace lve
