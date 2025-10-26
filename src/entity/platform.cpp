#include "platform.h"
#include "model/models.h"
#include <glm/gtc/matrix_transform.hpp>

namespace hack_game {

	using glm::vec3;
	using glm::mat4;

	Platform::Platform(Shader& shader, const glm::vec2& scale) noexcept:
			SimpleEntity(shader, models::platform),
			scale(scale) {}
	
	mat4 Platform::getModelTransform() const {
		return glm::scale(mat4(1.0f), vec3(scale.x, 0.0f, scale.y));
	}
}