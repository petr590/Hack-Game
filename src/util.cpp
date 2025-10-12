#include "util.h"
#include "entity/block.h"
#include "context/tick_context.h"

namespace hack_game {
	using std::min;
	using std::max;

	using glm::vec2;
	using glm::vec3;
	using glm::uvec2;

	static_assert(zoom(1, 0, 2, 6, 8) == 7);
	static_assert(zoom(150, 100, 200, 0, 1) == 0.5f);
	static_assert(invLerp(-100, 100, 0) == 0.5f);
	static_assert(invLerp(100, -100, 100) == 0);

	static_assert(isPointInsideSphere(vec3(0), vec3(0), 0));
	static_assert(isPointInsideSphere(vec3(0), vec3(1), 1.7320508f)); // sqrt(3)
	static_assert(!isPointInsideSphere(vec3(0), vec3(1), 1.7320507f));


	float horizontalAngleBetween(const vec3& pos, const vec3& lookAt) {
		vec2 dir(
			lookAt.x - pos.x,
			lookAt.z - pos.z
		);

		if (glm::length(dir) == 0) {
			return std::numeric_limits<float>::quiet_NaN();
		}

		return glm::orientedAngle(vec2(0.0f, 1.0f), glm::normalize(dir));
	}


	static vec2 resolveBlockCollision(const TickContext& context, const vec2& pos, vec2 offset, const uvec2& mapPos) {
		if (context.map[mapPos] == nullptr) {
			return offset;
		}

		AABB block = context.map[mapPos]->getHitbox();

		vec2 newPos = pos + offset;
		
		if (offset.x != 0 && block.containsInclusive(vec2(newPos.x, pos.y))) {
			if (offset.x > 0) offset.x = max(0.0f, offset.x + (block.min.x - newPos.x - EPSILON));
			else              offset.x = min(0.0f, offset.x + (block.max.x - newPos.x + EPSILON));
		}

		if (offset.y != 0 && block.containsInclusive(vec2(pos.x, newPos.y))) {
			if (offset.y > 0) offset.y = max(0.0f, offset.y + (block.min.y - newPos.y - EPSILON));
			else              offset.y = min(0.0f, offset.y + (block.max.y - newPos.y + EPSILON));
		}

		return offset;
	}


	vec2 resolveBlockCollision(const TickContext& context, const vec2& pos, vec2 offset) {
		const vec2 newPos = pos + offset;
		const uvec2 minPos = context.getMapPos(glm::min(pos, newPos) - EPSILON);
		const uvec2 maxPos = context.getMapPos(glm::max(pos, newPos) + EPSILON);

		offset = resolveBlockCollision(context, pos, offset, minPos);

		if (minPos.x != maxPos.x) {
			offset = resolveBlockCollision(context, pos, offset, uvec2(maxPos.x, minPos.y));
		}

		if (minPos.y != maxPos.y) {
			offset = resolveBlockCollision(context, pos, offset, uvec2(minPos.x, maxPos.y));

			if (minPos.x != maxPos.x) {
				offset = resolveBlockCollision(context, pos, offset, maxPos);
			}
		}

		return offset;
	}


	int randomBetween(int low, int high) {
		assert(low <= high);
		assert(low >= 0);
		assert(high <= RAND_MAX);
		return low + rand() % (high - low + 1);
	}

	float randomBetween(float low, float high) {
		return low + float(rand()) * (high - low) * (1.0f / RAND_MAX);
	}

	vec3 randomBetween(const vec3& low, const vec3& high) {
		return vec3(
			randomBetween(low.x, high.x),
			randomBetween(low.y, high.y),
			randomBetween(low.z, high.z)
		);
	}

	int32_t randomInt32() {
		return  ((rand() >> 3) << 24) |
				((rand() >> 3) << 12) |
				(rand() >> 3);
	}
}