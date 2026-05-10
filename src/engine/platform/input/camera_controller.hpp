#pragma once

#include <GLFW/glfw3.h>

#include "engine/core/lve_object.hpp"
namespace lve {

class CameraController {
  public:
	CameraController() = default;

	void initialize(GLFWwindow *window);
	void update(GLFWwindow *window, float dt, LVEObject &object);

	float moveSpeed{3.0f};

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
	KeyMapping keys{};

	float lookSpeed{1.5f};

	struct Mouse {
		float lastX = 0.0f;
		float lastY = 0.0f;

		float yaw =
			-90.0f; // yaw is initialized to -90.0 degrees since a yaw of 0.0
					// results in a direction vector pointing to the right so we
					// initially rotate a bit to the left.
		float pitch = 0.0f;
		float mouseSpeed = 0.001f;
	};
	Mouse mouse;

	float fov = 45.0f;

  private:
	void update_rotation(GLFWwindow *window, float dt, LVEObject &object);
	void update_position(GLFWwindow *window, float dt, LVEObject &object);
};

} // namespace lve
