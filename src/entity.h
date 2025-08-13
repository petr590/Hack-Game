#ifndef HACK_GAME_ENTITY_H
#define HACK_GAME_ENTITY_H

#include "draw_context.h"
#include "tick_context.h"
#include "model.h"
#include <glm/mat4x4.hpp>

namespace hack_game {

	class Entity {
	protected:
		constexpr Entity() = default;

	public:
		virtual ~Entity() = default;

		virtual void tick(TickContext&) = 0;
		virtual void draw() const = 0;
	};
}

#endif