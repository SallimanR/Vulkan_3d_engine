#include "vulkan_app.hpp"

#include "../engine/graphics/vulkan/vk_model.hpp"
#include "../engine/renderer/lve_camera.hpp"
#include "render_system.hpp"

#include <glm/trigonometric.hpp>
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
	LVECamera camera{};

	while (!lveWindow.should_close()) {
		glfwPollEvents();

		float aspectRatio = lveRenderer.get_aspect_ratio();
		// camera.set_orthographic_projection(-aspectRatio, aspectRatio, -1, 1,
		// -1, 								   1);

		camera.set_perspective_projection(glm::radians(50.f), aspectRatio, 0.1f,
										  10.0f);

		if (auto commandBuffer = lveRenderer.begin_frame()) {
			lveRenderer.begin_swap_chain_render_pass(*commandBuffer);
			renderSystem.render_objects(*commandBuffer, objects, camera);
			lveRenderer.end_swap_chain_render_pass(*commandBuffer);
			lveRenderer.end_frame();
		}
	}
}

std::shared_ptr<LVEVulkanModel> create_cube_model(LVEVulkanDevice &device,
												  glm::vec3 offset) {
	std::vector<LVEVulkanModel::Vertex> verticies{
		{{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
		{{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
		{{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
		{{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
		{{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},
		{{-.5f, .5f, .5f}, {.9f, .9f, .9f}},

		// right face (yellow)
		{{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
		{{.5f, .5f, .5f}, {.8f, .8f, .1f}},
		{{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
		{{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
		{{.5f, .5f, -.5f}, {.8f, .8f, .1f}},
		{{.5f, .5f, .5f}, {.8f, .8f, .1f}},

		// top face (orange, remember y axis points down)
		{{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
		{{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
		{{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
		{{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
		{{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
		{{.5f, -.5f, .5f}, {.9f, .6f, .1f}},

		// bottom face (red)
		{{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
		{{.5f, .5f, .5f}, {.8f, .1f, .1f}},
		{{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
		{{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
		{{.5f, .5f, -.5f}, {.8f, .1f, .1f}},
		{{.5f, .5f, .5f}, {.8f, .1f, .1f}},

		// nose face (blue)
		{{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
		{{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
		{{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
		{{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
		{{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
		{{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},

		// tail face (green)
		{{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
		{{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
		{{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
		{{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
		{{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
		{{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},

	};

	for (auto &v : verticies) {
		v.position += offset;
	}
	return std::make_unique<LVEVulkanModel>(device, verticies);
}

void LVEVulkanApp::load_objects() {
	auto lveModel = create_cube_model(lveVulkanDevice, {0.f, 0.f, 0.f});

	auto cube = LVEObject::create_object();
	cube.model = lveModel;
	// cube.color = {0.1f, 0.8f, 0.1f};
	cube.transform.translation = {0.0f, 0.0f, 2.5f};
	cube.transform.scale = {0.5f, 0.5f, 0.5f};
	// cube.transform.rotation = {0.0f, 0.0f, 0.0f};

	objects.push_back(std::move(cube));
}

} // namespace lve
