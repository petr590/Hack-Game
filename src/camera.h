#ifndef HACK_GAME_CAMERA_H
#define HACK_GAME_CAMERA_H

#include <glm/glm.hpp>

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