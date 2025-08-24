#include "animation_entity.h"
#include "enemy.h"
#include "../model/models.h"
#include "../context/tick_context.h"

#include <glm/gtc/matrix_transform.hpp>

namespace hack_game {

	AnimationEntity::AnimationEntity(DrawContext& drawContext) noexcept:
			SimpleEntity(drawContext, models::plane) {}
	

	void AnimationEntity::tick(TickContext& context) {
		pos = context.enemy->getPos();
		pos.y += 0.5f * TILE_SIZE;
	}


	glm::mat4 AnimationEntity::getModelTransform() const {
		glm::mat4 modelMat(1.0f);
		modelMat = glm::translate(modelMat, pos);
		modelMat = glm::scale(modelMat, glm::vec3(TILE_SIZE));
		return modelMat;
	}
}
