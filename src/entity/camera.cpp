#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace hack_game {
	using glm::vec3;
	using glm::mat4;

	void Camera::move(const vec3& offset) {
		pos += offset;
		target += offset;
		view = glm::lookAt(pos, target, vec3(0.0f, 1.0f, 0.0f));
	}
}
