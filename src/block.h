#ifndef HACK_GAME_BLOCK_H
#define HACK_GAME_BLOCK_H

#include "entity.h"
#include "models.h"
#include <glm/glm.hpp>

namespace hack_game {

	class Block: public Entity, public std::enable_shared_from_this<Entity> {
		const Model& model;
		float hitpoints;

	public:
		const glm::ivec2 pos;

		Block(DrawContext& drawContext, const Model& model, float hitpoints, const glm::ivec2& pos):
				Entity(drawContext), model(model), hitpoints(hitpoints), pos(pos) {}
		
		static inline Block breakable(DrawContext& drawContext, const glm::ivec2& pos) {
			return Block(drawContext, breakableCubeModel, 3.0f, pos);
		}
		
		static inline Block unbreakable(DrawContext& drawContext, const glm::ivec2& pos) {
			return Block(drawContext, unbreakableCubeModel, INFINITY, pos);
		}
		
		void tick(TickContext&) override;
		void draw() const override;
		void damage(TickContext&, float damage);
	};
}

#endif