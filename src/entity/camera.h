#ifndef HACK_GAME__ENTITY__CAMERA_H
#define HACK_GAME__ENTITY__CAMERA_H

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace hack_game {
	class Camera {
		glm::vec3 pos, target;
		glm::mat4 view;

	public:
		Camera(const glm::vec3& pos, const glm::vec3& target):
				pos(pos), target(target) {}
		
		const glm::vec3& getPos() const noexcept {
			return pos;
		}
		
		const glm::vec3& getTarget() const noexcept {
			return target;
		}

		const glm::mat4& getView() const noexcept {
			return view;
		}

		void move(const glm::vec3& offset);
	};
}

#endif
