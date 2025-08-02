#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

namespace hack_game {
	using glm::vec3;
	using glm::mat4;

	void Camera::move(const vec3& offset) {
		pos += offset;
		target += offset;
	}
	
	mat4 Camera::getView() const {
		return glm::lookAt(pos, target, vec3(0.0f, 1.0f, 0.0f));
	}
}