#include "animation.h"
#include "entity/enemy.h"
#include "entity/player.h"
#include "entity/entity_with_pos.h"
#include "model/models.h"
#include "context/shader.h"
#include "context/tick_context.h"
#include <glm/gtc/matrix_transform.hpp>

namespace hack_game {
	using std::shared_ptr;
	using std::move;

	using glm::vec3;
	using glm::vec4;
	using glm::mat3;
	using glm::mat4;

	Animation::Animation(shared_ptr<const EntityWithPos>&& entity, Shader& shader, Model& model, float size, float duration) noexcept:
			SimpleEntity(shader, model), entity(move(entity)), size(size), duration(duration) {}

	Animation::Animation(shared_ptr<const EntityWithPos>&& entity, Shader& shader, float size, float duration) noexcept:
			Animation(move(entity), shader, models::plane, size, duration) {}
	

	void Animation::tick(TickContext& context) {
		pos = entity->getPos();
		pos.y += Enemy::RADIUS; // TODO?

		time += context.deltaTime;

		if (time >= duration) {
			context.removeEntity(shared_from_this());
			onRemove();
		}
	}


	void Animation::draw() const {
		shader.setCenterPos(pos);
		shader.setProgress(time / duration);
		SimpleEntity::draw();
	}


	mat4 Animation::getModelTransform() const {
		mat4 modelMat(1.0f);
		modelMat = glm::translate(modelMat, pos);
		return glm::scale(modelMat, vec3(size));
	}
}