#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

namespace lve {

class LVECamera {
  public:
	void set_orthographic_projection(float left, float right, float top,
									 float bottom, float near, float far);
	void set_perspective_projection(float fovY, float aspect, float near,
									float far);

	void set_view_direction(glm::vec3 position, glm::vec3 direction,
							glm::vec3 up_direction = glm::vec3{0.0f, -1.0f,
															   0.0f});
	void set_view_target(glm::vec3 position, glm::vec3 target,
						 glm::vec3 up_direction = glm::vec3{0.0f, -1.0f, 0.0f});
	void set_view_yxz(glm::vec3 position, glm::vec3 rotation);

	const glm::mat4 &get_projection() const { return projectionMatrix; }
	const glm::mat4 &get_view() const { return viewMatrix; }

  private:
	glm::mat4 projectionMatrix{1.0f};
	glm::mat4 viewMatrix{1.0f};
};

} // namespace lve
