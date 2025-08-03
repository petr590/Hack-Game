#include "bullet.h"
#include <memory>
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
	using glm::quat;

	static const float LIMIT = 5.0f;
	static const float WIDTH = TILE_SIZE / 8;
	static const float HEIGHT = TILE_SIZE / 4;


	static constexpr bool hasCollision(float p1, float q1, float p2, float q2) {
		float min1 = min(p2, q2);
		float max1 = max(p2, q2);

		return  p1 >= min1 && p1 <= max1 ||
				q1 >= min1 && q1 <= max1 ||
				min(p1, q1) < min1 && max(p1, q1) > max1;
	}


	/**
	 * @param p1 - одна точка певой линии
	 * @param q1 - другая точка певой линии
	 * @param p1 - одна точка второй линии
	 * @param q1 - другая точка второй линии
	 * @return true, если линии пересекаются в точке или по отрезку, иначе false
	 */
	static constexpr bool hasCollision(
			const vec2& p1, const vec2& q1,
			const vec2& p2, const vec2& q2
	) {
		vec2 d1 = p1 - q1;
		vec2 d2 = p2 - q2;

		float c1 = d1.x == 0 ? INFINITY : d1.y / d1.x;
		float b1 = p1.y - p1.x * c1;

		float c2 = d2.x == 0 ? INFINITY : d2.y / d2.x;
		float b2 = p2.y - p2.x * c2;

		if (c1 == c2) {
			if (isinf(c1)) {
				return hasCollision(p1.y, q1.y, p2.y, q2.y);
			}

			return b1 == b2 && hasCollision(p1.x, q1.x, p2.x, q2.x);
		}

		float x = (b2 - b1) / (c1 - c2);
		return  x >= p1.x && x <= q1.x &&
				x >= p2.x && x <= q2.x;
	}

	static_assert(!hasCollision(vec2(0,1), vec2(1,2), vec2(4,4), vec2(2,2)));
	static_assert( hasCollision(vec2(0,1), vec2(1,2), vec2(1,2), vec2(2,2)));
	static_assert( hasCollision(vec2(0,0), vec2(1,1), vec2(0,1), vec2(1,0)));

	static_assert( hasCollision(vec2(0,0), vec2(0,1), vec2(0,1), vec2(0,2)));
	static_assert(!hasCollision(vec2(0,0), vec2(0,1), vec2(0,2), vec2(0,3)));
	
	static_assert( hasCollision(vec2(0,0), vec2(1,0), vec2(1,0), vec2(2,0)));
	static_assert(!hasCollision(vec2(0,0), vec2(1,0), vec2(2,0), vec2(3,0)));


	static vec2 rotateVec2(const vec2& vec, float angle) {
		float sinA, cosA;
		sincosf(angle, &sinA, &cosA);

		return vec2(
			vec.x * cosA - vec.y * sinA,
			vec.x * sinA + vec.y * cosA
		);
	}

	// static_assert(!hasCollision(vec2(0,0), vec2(0,0), vec2(0,1), vec2(0,1)));


	static bool checkCollision(const vec2& pos, float angle, TickContext& context) {
		const vec2 p1 = rotateVec2(vec2( WIDTH / 2,  HEIGHT / 2), angle) + pos;
		const vec2 p2 = rotateVec2(vec2( WIDTH / 2, -HEIGHT / 2), angle) + pos;
		const vec2 p3 = rotateVec2(vec2(-WIDTH / 2,  HEIGHT / 2), angle) + pos;
		const vec2 p4 = rotateVec2(vec2(-WIDTH / 2, -HEIGHT / 2), angle) + pos;

		const uint32_t minMapX = clamp(pos.x / TILE_SIZE - 0.5f, 0.0f, context.maxX());
		const uint32_t minMapY = clamp(pos.y / TILE_SIZE - 0.5f, 0.0f, context.maxY());
		const uint32_t maxMapX = clamp(pos.x / TILE_SIZE + 0.5f, 0.0f, context.maxX());
		const uint32_t maxMapY = clamp(pos.y / TILE_SIZE + 0.5f, 0.0f, context.maxY());

		printf("%u %u %u %u\n", minMapX, minMapY, maxMapX, maxMapY);

		for (uint32_t x = minMapX; x <= maxMapX; x++) {
			for (uint32_t y = minMapY; y <= maxMapY; y++) {
				if (!context.map[x][y]) continue;

				vec4 block = TickContext::getBlockHitbox(x, y);

				vec2 b1(block.x, block.y);
				vec2 b2(block.x, block.w);
				vec2 b3(block.z, block.y);
				vec2 b4(block.z, block.w);

				if (hasCollision(p1, p2, b1, b2) ||
					hasCollision(p1, p2, b3, b4) ||
					hasCollision(p1, p2, b1, b3) ||
					hasCollision(p1, p2, b2, b4) ||

					hasCollision(p3, p4, b1, b2) ||
					hasCollision(p3, p4, b3, b4) ||
					hasCollision(p3, p4, b1, b3) ||
					hasCollision(p3, p4, b2, b4) ||
					
					hasCollision(p1, p3, b1, b2) ||
					hasCollision(p1, p3, b3, b4) ||
					hasCollision(p1, p3, b1, b3) ||
					hasCollision(p1, p3, b2, b4) ||
					
					hasCollision(p2, p4, b1, b2) ||
					hasCollision(p2, p4, b3, b4) ||
					hasCollision(p2, p4, b1, b3) ||
					hasCollision(p2, p4, b2, b4)) {

					return true;
				}
			}
		}

		return false;
	}

	void Bullet::tick(TickContext& context) {
		pos += velocity * context.deltaTime;

		if (checkCollision(vec2(pos.x, pos.z), angle, context)) {
			context.removeEntity(shared_from_this());
		}

		if (pos.x > LIMIT || pos.x < -LIMIT ||
			pos.z > LIMIT || pos.z < -LIMIT) {

			context.removeEntity(shared_from_this());
		}
	}

	void Bullet::draw() const {
		Entity::draw();

		glm::mat4 modelMat(1.0f);
		modelMat = glm::translate(modelMat, pos);
		modelMat = glm::rotate(modelMat, angle, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(drawContext.modelLocation, 1, GL_FALSE, glm::value_ptr(modelMat));

		model.draw(drawContext);
	}
}