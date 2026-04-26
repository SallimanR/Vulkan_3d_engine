#include "lve_app_base.hpp"

#include <GLFW/glfw3.h>

namespace lve {

void LVEAppBase::run() {
	while (!lveWindow.shouldClose()) {
		glfwPollEvents();
	}
}

} //namespace lve
