#ifndef HACK_GAME_AABB_H
#define HACK_GAME_AABB_H

#include <glm/vec2.hpp>

namespace hack_game {
	struct AABB {
		glm::vec2 min;
		glm::vec2 max;

		constexpr AABB(const glm::vec2& min, const glm::vec2& max) noexcept:
				min(min), max(max) {};

		constexpr AABB(float minX, float minY, float maxX, float maxY) noexcept:
				min(minX, minY), max(maxX, maxY) {};
		

		constexpr bool containsInclusive(const glm::vec2& p) const noexcept {
			return  p.x >= min.x && p.x <= max.x &&
					p.y >= min.y && p.y <= max.y;
		}

		constexpr bool containsExclusive(const glm::vec2& p) const noexcept {
			return  p.x > min.x && p.x < max.x &&
					p.y > min.y && p.y < max.y;
		}
	};
}

#endif