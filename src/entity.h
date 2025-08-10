#ifndef HACK_GAME_ENTITY_H
#define HACK_GAME_ENTITY_H

#include "draw_context.h"
#include "tick_context.h"

namespace hack_game {

	class Entity {
	protected:
		DrawContext& drawContext;

		Entity(DrawContext& drawContext):
				drawContext(drawContext) {}

	public:
		virtual ~Entity() {}

		virtual void tick(TickContext&) = 0;
		virtual void draw() const {
			glUseProgram(drawContext.shaderProgram);
		}
	};
}

#endif