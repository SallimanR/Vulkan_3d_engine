#pragma once

#include <GLFW/glfw3.h>

#include "engine/core/lve_object.hpp"
namespace lve {

class KeyboardController {
  public:
	struct KeyMapping {
		int moveLeft = GLFW_KEY_A;
		int moveRight = GLFW_KEY_D;
		int moveForward = GLFW_KEY_W;
		int moveBackward = GLFW_KEY_S;
		int moveUp = GLFW_KEY_SPACE;
		int lookRight = GLFW_KEY_LEFT;
		int lookLeft = GLFW_KEY_RIGHT;
		int lookUp = GLFW_KEY_UP;
		int lookDown = GLFW_KEY_DOWN;
	};

	void move_in_plane_xz(GLFWwindow *window, float dt, LVEObject &object);

	KeyMapping keys{};
	float moveSpeed{3.0f};
	float lookSpeed{1.5f};
};

} // namespace lve
