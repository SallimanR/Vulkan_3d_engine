#include "lve_window.hpp"

#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>

namespace lve {

LVEWindow::LVEWindow(int w, int h, std::string name)
	: width{w}, height{h}, windowName{name} {
	initWindow();
}

LVEWindow::~LVEWindow() {
	glfwDestroyWindow(window);
	glfwTerminate();
}

void LVEWindow::initWindow() {
	glfwSetErrorCallback([](int error, const char *desc) {
		std::cerr << "GLFW error " << error << ": " << desc << std::endl;
	});

	if (!glfwInit()) {
		throw std::runtime_error("glfwInit() failed");
	}

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

	window =
		glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
	if (!window) {
		// glfwTerminate() already called inside destructor, but we should
		// clean up here because the constructor will abort.
		glfwTerminate();
		throw std::runtime_error("glfwCreateWindow() returned NULL");
	}
	glfwSetWindowUserPointer(window, this);
	glfwSetFramebufferSizeCallback(window, frameBufferResizedCallback);
}

void LVEWindow::createWindowSurface(VkInstance instace, VkSurfaceKHR *surface) {
	if (glfwCreateWindowSurface(instace, window, nullptr, surface) !=
		VK_SUCCESS) {
		throw std::runtime_error("failed to create window surface");
	}
}

void LVEWindow::frameBufferResizedCallback(GLFWwindow *window, int width,
										   int height) {
	auto lveWindow =
		reinterpret_cast<LVEWindow *>(glfwGetWindowUserPointer(window));
	lveWindow->isFrameBufferResized = true;
	lveWindow->width = width;
	lveWindow->height = height;
}

} // namespace lve
