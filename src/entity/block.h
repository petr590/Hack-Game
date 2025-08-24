#ifndef HACK_GAME__ENTITY__BLOCK_H
#define HACK_GAME__ENTITY__BLOCK_H

#include "simple_entity.h"
#include "aabb.h"
#include "../model/simple_model.h"
#include <memory>

namespace hack_game {

	class Block: public SimpleEntity, public std::enable_shared_from_this<Entity> {
		const glm::vec3 color;
		float hitpoints;
		float damageAnimationTime = 0;

	public:
		const glm::uvec2 pos;

		Block(DrawContext& drawContext, const SimpleModel& model, float hitpoints, const glm::uvec2& pos);
		
		static Block breakable(DrawContext& drawContext, const glm::uvec2& pos);
		static Block unbreakable(DrawContext& drawContext, const glm::uvec2& pos);

		AABB getHitbox() const;
		
		void damage(TickContext&, float damage);
		void tick(TickContext&) override;
		void draw() const override;
	};
}

#endif
