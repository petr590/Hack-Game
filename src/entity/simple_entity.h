#ifndef HACK_GAME__ENTITY__SIMPLE_ENTITY_H
#define HACK_GAME__ENTITY__SIMPLE_ENTITY_H

#include "entity.h"
#include <glm/mat4x4.hpp>

namespace hack_game {

	class SimpleEntity: public Entity {
	protected:
		DrawContext& drawContext;
		const Model& model;

	public:
		constexpr SimpleEntity(DrawContext& drawContext, const Model& model) noexcept:
				drawContext(drawContext), model(model) {}

		GLuint getShaderProgram() const override;
		void tick(TickContext&) override {}
		void draw() const override;
		virtual glm::mat4 getModelTransform() const;
	};
}

#endif