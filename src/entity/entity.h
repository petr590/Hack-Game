#ifndef HACK_GAME__ENTITY__ENTITY_H
#define HACK_GAME__ENTITY__ENTITY_H

#include "gl_fwd.h"
#include <glm/vec3.hpp>

namespace hack_game {

	class Model;
	class Shader;
	class DrawContext;
	class TickContext;

	class Entity {
	protected:
		constexpr Entity() = default;

	public:
		virtual ~Entity() = default;

		virtual GLuint getShaderProgram() const noexcept = 0;
		virtual void tick(TickContext&) = 0;
		virtual void draw() const = 0;

		virtual bool isTransparent() const noexcept {
			return false;
		}
	};
}

#endif
