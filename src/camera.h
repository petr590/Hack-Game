#ifndef HACK_GAME_CAMERA_H
#define HACK_GAME_CAMERA_H

#include <glm/glm.hpp>
#include <cassert>

namespace hack_game {
	class Camera {
		float speed;
		glm::vec3 pos, target;
		bool up, down, left, right;

	public:
		Camera(float speed, glm::vec3 pos, glm::vec3 target):
				speed(speed), pos(pos), target(target) {}

		void onKey(int key, int action);
		void move(float deltaTime);
		glm::mat4 getView() const;
	};
}

#endif