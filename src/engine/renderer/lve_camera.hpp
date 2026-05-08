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

	const glm::mat4 &get_projection() const { return projectionMatrix; }

  private:
	glm::mat4 projectionMatrix{1.0f};
};

} // namespace lve
