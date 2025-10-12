#include "billboard_animation.h"
#include "entity/player.h"
#include "context/tick_context.h"
#include <glm/gtc/matrix_transform.hpp>

namespace hack_game {

	using glm::vec3;
	using glm::vec4;
	using glm::mat3;
	using glm::mat4;

	void BillboardAnimation::tick(TickContext& context) {
		Animation::tick(context);
		view = context.player->getCamera().getView();
	}


	mat4 BillboardAnimation::getModelTransform() const {
		mat4 modelMat(glm::transpose(mat3(view)));
		modelMat[3] = vec4(getPos(), 1.0f);

		modelMat = glm::rotate(modelMat, glm::radians(90.0f), vec3(modelMat[0]));
		return glm::scale(modelMat, vec3(size));
	}
}