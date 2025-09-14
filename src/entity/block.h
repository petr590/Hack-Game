#ifndef HACK_GAME__ENTITY__BLOCK_H
#define HACK_GAME__ENTITY__BLOCK_H

#include "simple_entity.h"
#include "damageable.h"
#include "aabb.h"
#include <memory>

namespace hack_game {

	class ColoredModel;


	class Block: public SimpleEntity, public Damageable {
		const ColoredModel& coloredModel;
		float damageAnimationTime = 0;

	public:
		const glm::uvec2 pos;

		Block(Shader& shader, const ColoredModel& model, hp_t hitpoints, const glm::uvec2& pos);
		
		static Block breakable(Shader& shader, const glm::uvec2& pos);
		static Block unbreakable(Shader& shader, const glm::uvec2& pos);

		AABB getHitbox() const;
		
		bool hasCollision(const glm::vec3& point) const override;
		void damage(TickContext&, hp_t damage) override;
		void tick(TickContext&) override;
		void draw() const override;
	
	protected:
		void onDestroy(TickContext&) override;
	};
}

#endif