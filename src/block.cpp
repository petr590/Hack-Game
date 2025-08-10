#include "block.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace hack_game {
	using glm::vec2;
	using glm::vec3;
	using glm::mat4;

	AABB Block::getHitbox() const {
		vec2 center = (vec2(pos) + 0.5f) * TILE_SIZE;

		return AABB(
			center - TILE_SIZE / 2,
			center + TILE_SIZE / 2
		);
	}

	void Block::tick(TickContext&) {}

	void Block::draw() const {
		Entity::draw();

		mat4 model(1.0f);

		model = glm::translate(model, vec3(
			TILE_SIZE * (pos.x + 0.5f),
			0.0f,
			TILE_SIZE * (pos.y + 0.5f)
		));

		glUniformMatrix4fv(drawContext.modelLocation, 1, GL_FALSE, glm::value_ptr(model));

		this->model.draw(drawContext);
	}

	void Block::damage(TickContext& context, float damage) {
		hitpoints -= damage;

		if (hitpoints <= 0) {
			context.map[pos] = nullptr;
			context.removeEntity(shared_from_this());
		}
	}
}