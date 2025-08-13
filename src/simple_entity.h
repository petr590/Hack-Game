#ifndef HACK_GAME_ONE_MODEL_ENTITY_H
#define HACK_GAME_ONE_MODEL_ENTITY_H

#include "entity.h"

namespace hack_game {
	class SimpleEntity: public Entity {
	protected:
		DrawContext& drawContext;
		const Model& model;

		constexpr SimpleEntity(DrawContext& drawContext, const Model& model) noexcept:
				drawContext(drawContext), model(model) {}

	public:
		void draw() const override;
		virtual glm::mat4 getModelTransform() const;
	};
}

#endif