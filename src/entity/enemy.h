#ifndef HACK_GAME__ENTITY__ENEMY_H
#define HACK_GAME__ENTITY__ENEMY_H

#include "simple_entity.h"
#include "damageable.h"
#include "entity_with_pos.h"
#include <memory>

namespace hack_game {

	class Animation;
	class ColoredModel;


	class Enemy: public SimpleEntity, public Damageable, public EntityWithPos {
	public:
		static constexpr float RADIUS = 0.02f;

	protected:
		ShaderManager& shaderManager;
		const ColoredModel& coloredModel;
		std::shared_ptr<Animation> animation = nullptr;

		const float bulletSpawnPeriod;
		glm::vec3 pos;
		float time = 0;

	public:
		Enemy(ShaderManager&, float bulletSpawnPeriod, const glm::vec3& pos) noexcept;

		const glm::vec3& getPos() const noexcept override {
			return pos;
		}
		
		bool hasCollision(const glm::vec3& point) const override;
		void damage(Level&, hp_t damage) override;
		void tick(Level&) override;
		void draw() const override;
		glm::mat4 getModelTransform() const override;

		std::shared_ptr<const Enemy> shared_from_this() const;
	
	protected:
		virtual void spawnBullets(Level&) = 0;
	};


	class Enemy1: public Enemy {
		float angle = 0;
		bool spawnUnbreakable = false;

	public:
		Enemy1(ShaderManager&, const glm::vec3& pos) noexcept;
		
		void tick(Level&) override;

	protected:
		void spawnBullets(Level&) override;
	};
}

#endif
