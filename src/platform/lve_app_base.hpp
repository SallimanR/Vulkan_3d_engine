#pragma once

#include "lve_window.hpp"

namespace lve {

class LVEAppBase {
public:
	static constexpr int WIDTH = 800;
	static constexpr int HEIGHT = 600;

	void run();

private:
	LVEWindow lveWindow{ WIDTH, HEIGHT, "C++ Vulkan" };
};

} //namespace lve
