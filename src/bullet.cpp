#include "bullet.h"
#include "block.h"
#include <algorithm>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace hack_game {
	using std::min;
	using std::max;
	using std::clamp;
	using std::isinf;

	using glm::uvec2;
	using glm::vec2;
	using glm::vec3;
	using glm::mat4;

	static const float LIMIT = 5.0f;
	static const float WIDTH = TILE_SIZE / 8;
	static const float HEIGHT = TILE_SIZE / 4;


	static bool checkCollision(const vec2& pos, TickContext& context) {
		const uvec2 mapPos = context.getMapPos(pos);

		if (context.map[mapPos] == nullptr) {
			return false;
		}

		const AABB block = context.map[mapPos]->getHitbox();

		if (block.containsInclusive(pos)) {
			context.map[mapPos]->damage(context, 1.0f);
			return true;
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