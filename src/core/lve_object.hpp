#pragma once

#include "../graphics/vulkan/vk_model.hpp"
#include <glm/ext/matrix_float2x2.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/trigonometric.hpp>
#include <memory>

namespace lve {

struct Transform2dComponent {
	glm::vec2 translation{}; // position offset
	glm::vec2 scale{1.f, 1.f};
	float rotation;

	glm::mat2 mat2() {
		const float sinRotation = glm::sin(rotation);
		const float cosRotation = glm::cos(rotation);

		// NOTE: Takes matrices takes columns, not rows
		glm::mat2 rotationMatrix{{cosRotation, sinRotation},
								 {-sinRotation, cosRotation}};
		glm::mat2 scaleMatrix{{scale.x, .0f}, {.0f, scale.y}};
		// ------------

		glm::mat2 transformations = rotationMatrix * scaleMatrix;
		return transformations;
	}
};

// Currently contains all fields of engine
class LVEObject {
  public:
	using id_t = unsigned int;

	static LVEObject createObject() {
		static id_t currentId = 0;
		return LVEObject{currentId++};
	}

	LVEObject(const LVEObject &) = delete;
	LVEObject &operator=(const LVEObject &) = delete;
	LVEObject(LVEObject &&) = default;
	LVEObject &operator=(LVEObject &&) = default;

	id_t getIDd() { return id; }

	std::shared_ptr<LVEVulkanModel> model{};
	glm::vec3 color{};
	Transform2dComponent transform2d{};

  private:
	LVEObject(id_t objId) : id{objId} {}

	id_t id;
};
} // namespace lve
