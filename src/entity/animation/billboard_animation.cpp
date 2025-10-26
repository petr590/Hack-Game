#include "billboard_animation.h"
#include "entity/player.h"
#include "level/level.h"
#include <glm/gtc/matrix_transform.hpp>

namespace hack_game {

	using glm::vec3;
	using glm::vec4;
	using glm::mat3;
	using glm::mat4;

	void BillboardAnimation::tick(Level& level) {
		Animation::tick(level);
		view = level.getPlayer()->getCamera().getView();
	}


	mat4 BillboardAnimation::getModelTransform() const {
		mat4 modelMat(glm::transpose(mat3(view)));
		modelMat[3] = vec4(getPos(), 1.0f);

		modelMat = glm::rotate(modelMat, glm::radians(90.0f), vec3(modelMat[0]));
		return glm::scale(modelMat, vec3(size));
	}
}