#ifndef HACK_GAME__ENTITY__ENTITY_H
#define HACK_GAME__ENTITY__ENTITY_H

#include "../gl_fwd.h"

namespace hack_game {

	class Model;
	class DrawContext;
	class TickContext;

	class Entity {
	protected:
		constexpr Entity() = default;

	public:
		virtual ~Entity() = default;

		virtual void tick(TickContext&) = 0;
		virtual void draw() const = 0;
		virtual GLuint getShaderProgram() const = 0;
	};
}

#endif
