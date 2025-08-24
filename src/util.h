#ifndef HACK_GAME__UTIL_H
#define HACK_GAME__UTIL_H

#include <glm/vec3.hpp>

namespace hack_game {
	static constexpr glm::vec3 colorAsVec3(uint32_t color) {
		return glm::vec3(
			((color >> 16) & 0xFF) / 255.0f,
			((color >>  8) & 0xFF) / 255.0f,
			((color      ) & 0xFF) / 255.0f
		);
	}
}

#endif