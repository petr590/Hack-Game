#ifndef HACK_GAME_VERTEX_H
#define HACK_GAME_VERTEX_H

#include <glm/vec3.hpp>

namespace hack_game {
	struct Vertex {
		const glm::vec3 pos;
		const glm::vec3 normal;

		constexpr Vertex(const glm::vec3 pos, const glm::vec3 normal) noexcept:
			pos(pos), normal(normal) {}
	};
}

#endif