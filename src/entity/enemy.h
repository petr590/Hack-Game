#ifndef HACK_GAME__ENTITY__ENEMY_H
#define HACK_GAME__ENTITY__ENEMY_H

#include "simple_entity.h"
#include "damageable.h"
#include <memory>

namespace hack_game {

	class Animation;
	class ColoredModel;


	class Enemy: public SimpleEntity, public Damageable {
	public:
		static constexpr float RADIUS = 0.02f;

	protected:
		DrawContext& drawContext;
		const ColoredModel& coloredModel;
		const float bulletSpawnPeriod;

		glm::vec3 pos;
		float time = 0;
		std::shared_ptr<Animation> animation = nullptr;

	public:
		Enemy(DrawContext&, float bulletSpawnPeriod, const glm::vec3& pos) noexcept;

		const glm::vec3& getPos() const noexcept {
			return pos;
		}
		
		bool hasCollision(const glm::vec3& point) const override;
		void damage(TickContext&, hp_t damage) override;
		void tick(TickContext&) override;
		void draw() const override;
		glm::mat4 getModelTransform() const override;
	
	protected:
		virtual void spawnBullets(TickContext&) = 0;
	};


	class Enemy1: public Enemy {
		float angle = 0;
		bool spawnUnbreakable = false;

	public:
		Enemy1(DrawContext&, const glm::vec3& pos) noexcept;
		
		void tick(TickContext&) override;

	protected:
		void spawnBullets(TickContext&) override;
	};
}

#endif
