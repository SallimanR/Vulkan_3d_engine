#pragma once

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

	bool shouldClose() { return glfwWindowShouldClose(window); }
	VkExtent2D getExtent() {
		return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};
	}
	bool wasWindowResized() { return isFrameBufferResized; }
	void resetWindowResizedFlag() { isFrameBufferResized = false; }

	void createWindowSurface(VkInstance instace, VkSurfaceKHR *surface);

  private:
	void initWindow();
	static void frameBufferResizedCallback(GLFWwindow *window, int width,
										   int height);

	int width;
	int height;
	bool isFrameBufferResized = false;

	std::string windowName;
	GLFWwindow *window;
};

} // namespace lve
