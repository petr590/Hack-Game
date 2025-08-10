#ifndef HACK_GAME_BLOCK_H
#define HACK_GAME_BLOCK_H

#include "aabb.h"
#include "entity.h"
#include "models.h"
#include <limits>

namespace hack_game {

	class Block: public Entity, public std::enable_shared_from_this<Entity> {
		const Model& model;
		float hitpoints;

	public:
		const glm::uvec2 pos;

		Block(DrawContext& drawContext, const Model& model, float hitpoints, const glm::uvec2& pos):
				Entity(drawContext), model(model), hitpoints(hitpoints), pos(pos) {}
		
		static inline Block breakable(DrawContext& drawContext, const glm::uvec2& pos) {
			return Block(drawContext, breakableCubeModel, 3.0f, pos);
		}
		
		static inline Block unbreakable(DrawContext& drawContext, const glm::uvec2& pos) {
			return Block(drawContext, unbreakableCubeModel, std::numeric_limits<float>().infinity(), pos);
		}

		AABB getHitbox() const;
		
		void tick(TickContext&) override;
		void draw() const override;
		void damage(TickContext&, float damage);
	};
}

#endif