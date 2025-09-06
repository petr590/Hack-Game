#ifndef HACK_GAME__ENTITY__BULLET_H
#define HACK_GAME__ENTITY__BULLET_H

#include "simple_entity.h"
#include "damageable.h"
#include <memory>

namespace hack_game {

	class Bullet: public SimpleEntity, public virtual std::enable_shared_from_this<Entity> {
	protected:
		const float angle;
		const glm::vec3 velocity;
		glm::vec3 pos;

	public:
		Bullet(Shader& shader, const Model& model, float angle, const glm::vec3& velocity, const glm::vec3& pos) noexcept;

		const glm::vec3& getPos() const noexcept {
			return pos;
		}
		
		void tick(TickContext&) override;
		glm::mat4 getModelTransform() const override;
	
	protected:
		/**
		 * Проверяет коллизию с одним из объектов в сцене и наносит ему урон, если есть коллизия
		 * @return true, если есть коллизия, иначе false
		 */
		virtual bool checkCollision(TickContext&) = 0;
	};


	class PlayerBullet: public Bullet {
	public:
		PlayerBullet(Shader& shader, float angle, const glm::vec3& velocity, const glm::vec3& pos);

	protected:
		bool checkCollision(TickContext&) override;
	};


	class EnemyBullet: public Bullet, public Damageable {
	public:
		static constexpr float DEFAULT_SPEED = 0.15f;

		EnemyBullet(Shader& shader, bool unbreakable, const glm::vec3& velocity, const glm::vec3& pos);
	
		glm::mat4 getModelTransform() const override;
		bool hasCollision(const glm::vec3& point) const override;

	protected:
		void onDestroy(TickContext&) override;
		bool checkCollision(TickContext&) override;
	};
}

#endif
