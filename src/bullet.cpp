#include "bullet.h"
#include "block.h"
#include <memory>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace hack_game {
	using std::min;
	using std::max;
	using std::clamp;
	using std::isinf;

	using glm::vec2;
	using glm::vec3;
	using glm::vec4;
	using glm::mat4;
	using glm::quat;

	static const float LIMIT = 5.0f;
	static const float WIDTH = TILE_SIZE / 8;
	static const float HEIGHT = TILE_SIZE / 4;


	static bool checkCollision(const vec2& pos, TickContext& context) {
		const uint32_t minMapX = clamp(pos.x / TILE_SIZE - 0.5f, 0.0f, context.maxX());
		const uint32_t minMapY = clamp(pos.y / TILE_SIZE - 0.5f, 0.0f, context.maxY());
		const uint32_t maxMapX = clamp(pos.x / TILE_SIZE + 0.5f, 0.0f, context.maxX());
		const uint32_t maxMapY = clamp(pos.y / TILE_SIZE + 0.5f, 0.0f, context.maxY());

		for (uint32_t x = minMapX; x <= maxMapX; x++) {
			for (uint32_t y = minMapY; y <= maxMapY; y++) {
				if (context.map[x][y] == nullptr) continue;

				vec4 block = TickContext::getBlockHitbox(x, y);

				if (pos.x >= block.x && pos.x <= block.z &&
					pos.y >= block.y && pos.y <= block.w) {
					
					context.map[x][y]->damage(context, 1.0f);
					return true;
				}
			}
		}

		return false;
	}

	void Bullet::tick(TickContext& context) {
		pos += velocity * context.deltaTime;

		if (checkCollision(vec2(pos.x, pos.z), context)) {
			context.removeEntity(shared_from_this());
		}

		if (pos.x > LIMIT || pos.x < -LIMIT ||
			pos.z > LIMIT || pos.z < -LIMIT) {

			context.removeEntity(shared_from_this());
		}
	}

	void Bullet::draw() const {
		Entity::draw();

		mat4 modelMat(1.0f);
		modelMat = glm::translate(modelMat, pos);
		modelMat = glm::rotate(modelMat, angle, vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(drawContext.modelLocation, 1, GL_FALSE, glm::value_ptr(modelMat));

		model.draw(drawContext);
	}
}