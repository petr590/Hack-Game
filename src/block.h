#ifndef HACK_GAME_BLOCK_H
#define HACK_GAME_BLOCK_H

#include "aabb.h"
#include "simple_entity.h"
#include "models.h"
#include <limits>

namespace hack_game {

	class Block: public SimpleEntity, public std::enable_shared_from_this<Entity> {
		const glm::vec3 color;
		float hitpoints;
		float damageAnimationTime = 0;

	public:
		const glm::uvec2 pos;

		Block(DrawContext& drawContext, const SimpleModel& model, float hitpoints, const glm::uvec2& pos);
		
		static Block breakable(DrawContext& drawContext, const glm::uvec2& pos) {
			return Block(drawContext, models::breakableCube, 3.0f, pos);
		}
		
		static Block unbreakable(DrawContext& drawContext, const glm::uvec2& pos) {
			return Block(drawContext, models::unbreakableCube, std::numeric_limits<float>().infinity(), pos);
		}

		AABB getHitbox() const;
		
		void damage(TickContext&, float damage);
		void tick(TickContext&) override;
		void draw() const override;
	};
}

#endif