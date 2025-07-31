#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

namespace hack_game {
	using glm::vec3;
	using glm::mat4;

	void Camera::onKey(int key, int action) {
		if (action != GLFW_PRESS && action != GLFW_RELEASE) return;

		bool value = action == GLFW_PRESS;
		switch (key) {
			case GLFW_KEY_W: up = value; break;
			case GLFW_KEY_S: down = value; break;
			case GLFW_KEY_A: left = value; break;
			case GLFW_KEY_D: right = value; break;
		}
	}

	void Camera::move(float deltaTime) {
		vec3 offset(
			left ? -1 : right ? 1 : 0,
			0,
			up ? -1 : down ? 1 : 0
		);

		if (glm::length(offset) == 0) return;

		offset = glm::normalize(offset) * speed * deltaTime;
		pos += offset;
		target += offset;
	}
	
	mat4 Camera::getView() const {
		return glm::lookAt(pos, target, vec3(0.0f, 1.0f, 0.0f));
	}
}