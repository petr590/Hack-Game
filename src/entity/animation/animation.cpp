#include "animation.h"
#include "entity/enemy.h"
#include "entity/player.h"
#include "model/models.h"
#include "context/draw_context.h"
#include "context/tick_context.h"
#include <glm/gtc/matrix_transform.hpp>

namespace hack_game {
	using glm::vec3;
	using glm::vec4;
	using glm::mat3;
	using glm::mat4;

	Animation::Animation(Shader& shader, float size, float duration) noexcept:
			SimpleEntity(shader, models::plane), size(size), duration(duration) {}
	

	void Animation::tick(TickContext& context) {
		pos = context.enemy->getPos();
		pos.y += Enemy::RADIUS;

		viewMat3 = mat3(context.player->getCamera().getView());

		time += context.deltaTime;

		if (time >= duration) {
			context.removeEntity(shared_from_this());
		}
	}


	void Animation::draw() const {
		shader.setCenterPos(pos);
		shader.setProgress(time / duration);
		SimpleEntity::draw();
	}


	mat4 Animation::getModelTransform() const {
		mat4 modelMat(glm::transpose(viewMat3));
		modelMat[3] = vec4(pos, 1.0f);

		modelMat = glm::rotate(modelMat, glm::radians(90.0f), vec3(modelMat[0]));
		return glm::scale(modelMat, vec3(size));
	}
}