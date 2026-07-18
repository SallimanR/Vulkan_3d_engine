#pragma once

#include <vulkan/vulkan_core.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace lve {

class LVEWindow {
  public:
	LVEWindow(int w, int h, std::string name);
	~LVEWindow();

	// Delete constructor and copy operators.
	// We don't want 2 pointers to same window at the same time.
	LVEWindow(const LVEWindow &) = delete;
	LVEWindow &operator=(const LVEWindow &) = delete;

	bool should_close() { return glfwWindowShouldClose(window); }
	VkExtent2D get_extent() {
		return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};
	}
	bool was_window_resized() { return isFrameBufferResized; }
	void reset_window_resized_flag() { isFrameBufferResized = false; }

	void create_window_surface(VkInstance instance, VkSurfaceKHR *surface);

	GLFWwindow *get_window_ptr() const { return window; }

  private:
	void init_window();
	static void frame_buffer_resized_callback(GLFWwindow *window, int width,
											  int height);

	int width;
	int height;
	bool isFrameBufferResized = false;

	std::string windowName;
	GLFWwindow *window;
};

} // namespace lve
