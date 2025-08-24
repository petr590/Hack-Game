#ifndef HACK_GAME__ENTITY__BULLET_H
#define HACK_GAME__ENTITY__BULLET_H

#include "simple_entity.h"
#include <memory>

namespace hack_game {

	class Bullet: public SimpleEntity, public std::enable_shared_from_this<Entity> {
	protected:
		const float angle;
		const glm::vec3 velocity;
		glm::vec3 pos;

	public:
		Bullet(DrawContext& drawContext, const Model& model, float angle, const glm::vec3& velocity, const glm::vec3& pos) noexcept;

		const glm::vec3& getPos() const noexcept {
			return pos;
		}
		
		void tick(TickContext&) override;
		glm::mat4 getModelTransform() const override;
	
	protected:
		virtual bool checkCollision(TickContext&) = 0;
	};


	class PlayerBullet: public Bullet {
	public:
		PlayerBullet(DrawContext& drawContext, float angle, const glm::vec3& velocity, const glm::vec3& pos);

	protected:
		bool checkCollision(TickContext&) override;
	};


	class EnemyBullet: public Bullet {
	public:
		const bool unbreakable;

		EnemyBullet(DrawContext& drawContext, bool unbreakable, const glm::vec3& velocity, const glm::vec3& pos);

	protected:
		bool checkCollision(TickContext&) override;
	};
}

#endif
