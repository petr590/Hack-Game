#ifndef HACK_GAME__ENTITY__CAMERA_H
#define HACK_GAME__ENTITY__CAMERA_H

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace hack_game {
	class Camera {
	public:
		glm::vec3 pos, target;

		Camera(const glm::vec3& pos, const glm::vec3& target):
				pos(pos), target(target) {}

		void move(const glm::vec3& offset);
		glm::mat4 getView() const;
	};
}

#endif
