#pragma once

#include "../graphics/vulkan/vk_model.hpp"
#include <glm/ext/matrix_float2x2.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/trigonometric.hpp>
#include <memory>

namespace lve {

struct TransformComponent {
	glm::vec3 translation{}; // position offset
	glm::vec3 scale{1.f, 1.f, 1.f};
	glm::vec3 rotation{};

	// Matrix corresponds to: "translate * Ry * Rx * Rz * scale" transformation
	// Rotation uses tait-bryan angles with axis order Y(1), X(2), Z(3)
	glm::mat4 mat4() {
		auto transform = glm::translate(glm::mat4{1.0f}, translation);

		// Order is important
		transform = glm::rotate(transform, rotation.y, {0.0f, 1.0f, 0.0f});
		transform = glm::rotate(transform, rotation.x, {1.0f, 0.0f, 0.0f});
		transform = glm::rotate(transform, rotation.z, {0.0f, 0.0f, 1.0f});

		transform = glm::scale(transform, scale);
		return transform;
	}
};

class LVEObject {
  public:
	using id_t = unsigned int;

	static LVEObject create_object() {
		static id_t currentId = 0;
		return LVEObject{currentId++};
	}

	LVEObject(const LVEObject &) = delete;
	LVEObject &operator=(const LVEObject &) = delete;
	LVEObject(LVEObject &&) = default;
	LVEObject &operator=(LVEObject &&) = default;

	id_t get_id() const { return id; }

	std::shared_ptr<LVEVulkanModel> model{};
	glm::vec3 color{};
	TransformComponent transform{};

  private:
	LVEObject(id_t objId) : id{objId} {}

	id_t id;
};

} // namespace lve
