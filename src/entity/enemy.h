#ifndef HACK_GAME__ENTITY__ENEMY_H
#define HACK_GAME__ENTITY__ENEMY_H

#include "simple_entity.h"

namespace hack_game {

	class Enemy: public SimpleEntity {
	public:
		static constexpr float RADIUS = 0.02f;

	protected:
		DrawContext& bulletDrawContext;

		glm::vec3 pos;
		const float bulletSpawnPeriod;
		float time = 0;
		float hitpoints = 7.0f;

	public:
		Enemy(DrawContext& drawContext, DrawContext& bulletDrawContext, const glm::vec3& pos, float bulletSpawnPeriod) noexcept;

		const glm::vec3 getPos() const noexcept {
			return pos;
		}
		
		void damage(TickContext&, float damage);
		void tick(TickContext&) override;
		glm::mat4 getModelTransform() const override;
	
	protected:
		virtual void spawnBullets(TickContext&) = 0;
	};


	class Enemy1: public Enemy {
		float angle = 0;
		bool spawnUnbreakable = false;

	public:
		Enemy1(DrawContext& drawContext, DrawContext& bulletDrawContext, const glm::vec3& pos) noexcept;
		
		void tick(TickContext&) override;

	protected:
		void spawnBullets(TickContext&) override;
	};
}

#endif
