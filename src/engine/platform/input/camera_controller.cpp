#include "camera_controller.hpp"

#include <GLFW/glfw3.h>
#include <glm/common.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/constants.hpp>

#include <limits>

namespace lve {

inline static bool _is_vector_non_zero(glm::vec3 vector) {
	return glm::dot(vector, vector) > std::numeric_limits<float>::epsilon();
}

void CameraController::initialize(GLFWwindow *window) {
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
};

void CameraController::update(GLFWwindow *window, float dt, LVEObject &object) {
	update_rotation(window, dt, object);
	update_position(window, dt, object);
};

void CameraController::update_position(GLFWwindow *window, float dt,
									   LVEObject &object) {
	float yaw = object.transform.rotation.y;
	const glm::vec3 forwardDir{sin(yaw), 0.0f, cos(yaw)};
	const glm::vec3 rightDir{forwardDir.z, 0.0f, -forwardDir.x};
	const glm::vec3 upDir{0.0f, -1.0f, 0.0f};

	glm::vec3 moveDir{0.0f, 0.0f, 0.0f};
	if (glfwGetKey(window, keys.moveForward) == GLFW_PRESS)
		moveDir += forwardDir;
	if (glfwGetKey(window, keys.moveBackward) == GLFW_PRESS)
		moveDir -= forwardDir;
	if (glfwGetKey(window, keys.moveRight) == GLFW_PRESS)
		moveDir += rightDir;
	if (glfwGetKey(window, keys.moveLeft) == GLFW_PRESS)
		moveDir -= rightDir;
	if (glfwGetKey(window, keys.moveUp) == GLFW_PRESS &&
		glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) != GLFW_PRESS)
		moveDir += upDir;
	if (glfwGetKey(window, keys.moveUp) == GLFW_PRESS &&
		glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		moveDir -= upDir;

	if (_is_vector_non_zero(moveDir)) {
		object.transform.translation +=
			moveSpeed * dt * glm::normalize(moveDir);
	}
}

void CameraController::update_rotation(GLFWwindow *window, float dt,
									   LVEObject &object) {
	/* Handle mouse input */
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	float xoffset = static_cast<float>(xpos) - mouse.lastX;
	float yoffset =
		mouse.lastY - static_cast<float>(ypos); // reversed: y goes up on screen
	mouse.lastX = static_cast<float>(xpos);
	mouse.lastY = static_cast<float>(ypos);

	xoffset *= mouse.mouseSpeed;
	yoffset *= mouse.mouseSpeed;

	object.transform.rotation.y += xoffset;
	object.transform.rotation.x += yoffset;
	// limit pitch values between about +/- 85ish degrees
	object.transform.rotation.x =
		glm::clamp(object.transform.rotation.x, -1.5f, 1.5f);
	object.transform.rotation.y =
		glm::mod(object.transform.rotation.y, glm::two_pi<float>());

	/* Handle keyboard input */
	glm::vec3 rotate{0.0f, 0.0f, 0.0f};
	if (glfwGetKey(window, keys.lookRight) == GLFW_PRESS)
		rotate.y += 1.0f;
	if (glfwGetKey(window, keys.lookLeft) == GLFW_PRESS)
		rotate.y -= 1.0f;
	if (glfwGetKey(window, keys.lookUp) == GLFW_PRESS)
		rotate.x += 1.0f;
	if (glfwGetKey(window, keys.lookDown) == GLFW_PRESS)
		rotate.x -= 1.0f;

	if (_is_vector_non_zero(rotate)) {
		object.transform.rotation += lookSpeed * dt * glm::normalize(rotate);
	}
}

} // namespace lve
