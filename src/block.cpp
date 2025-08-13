#include "block.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace hack_game {
	using std::max;

	using glm::vec2;
	using glm::vec3;
	using glm::mat4;

	Block::Block(DrawContext& drawContext, const SimpleModel& model, float hitpoints, const glm::uvec2& pos):
			SimpleEntity(drawContext, model), color(model.color), hitpoints(hitpoints), pos(pos) {}

	AABB Block::getHitbox() const {
		vec2 center = (vec2(pos) + 0.5f) * TILE_SIZE;

		return AABB(
			center - TILE_SIZE / 2,
			center + TILE_SIZE / 2
		);
	}


	const float DAMAGE_ANIMATION_DURATION = 0.25f;

	void Block::damage(TickContext& context, float damage) {
		if (hitpoints == std::numeric_limits<float>().infinity()) {
			return;
		}

		if (damageAnimationTime <= 0) {
			damageAnimationTime = DAMAGE_ANIMATION_DURATION;
		}
		
		hitpoints -= damage;

		if (hitpoints <= 0) {
			context.map[pos] = nullptr;
		}
	}

	void Block::tick(TickContext& context) {
		if (damageAnimationTime > 0) {
			damageAnimationTime -= context.deltaTime;

			if (damageAnimationTime <= 0 && hitpoints <= 0) {
				context.removeEntity(shared_from_this());
			}
		}
	}

	void Block::draw() const {
		mat4 modelMat(1.0f);
		modelMat = glm::translate(modelMat, vec3(
			(pos.x + 0.5f) * TILE_SIZE,
			0.0f,
			(pos.y + 0.5f) * TILE_SIZE
		));

		glUseProgram(drawContext.shaderProgram);
		glUniformMatrix4fv(drawContext.modelLocation, 1, GL_FALSE, glm::value_ptr(modelMat));

		float brightness = max(0.0f, damageAnimationTime * (1.25f / DAMAGE_ANIMATION_DURATION));
		model.draw(drawContext, color + brightness);
	}
}