#include "lve_window.hpp"

#include <GLFW/glfw3.h>

#include <iostream>

namespace lve {

LVEWindow::LVEWindow(int w, int h, std::string name) : width{ w }, height{ h }, windowName{ name } {
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
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
	if (!window) {
		// glfwTerminate() already called inside destructor, but we should
		// clean up here because the constructor will abort.
		glfwTerminate();
		throw std::runtime_error("glfwCreateWindow() returned NULL");
	}
}

} //namespace lve
