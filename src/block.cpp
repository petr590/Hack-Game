#include "block.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace hack_game {
	void Block::tick(TickContext&) {}

	void Block::draw() const {
		Entity::draw();

		glm::mat4 model(1.0f);

		model = glm::translate(model, glm::vec3(
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
			context.map[pos.x][pos.y] = nullptr;
			context.removeEntity(shared_from_this());
		}
	}
}