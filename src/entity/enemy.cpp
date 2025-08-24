#include "enemy.h"
#include "player.h"
#include "bullet.h"
#include "../model/models.h"
#include "../context/tick_context.h"
#include "../globals.h"

#include <glm/gtx/vector_angle.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace hack_game {
	using std::clamp;
	using std::make_shared;

	using glm::vec2;
	using glm::vec3;
	using glm::mat4;

	Enemy::Enemy(DrawContext& drawContext, DrawContext& bulletDrawContext, const glm::vec3& pos, float bulletSpawnPeriod) noexcept:
				SimpleEntity(drawContext, models::sphere),
				bulletDrawContext(bulletDrawContext),
				pos(pos),
				bulletSpawnPeriod(bulletSpawnPeriod) {}


	void Enemy::damage(TickContext& context, float damage) {
		hitpoints -= damage;

		if (hitpoints <= 0) {
			won = true;
		}
	}

	void Enemy::tick(TickContext& context) {
		time += context.deltaTime;

		if (time >= bulletSpawnPeriod) {
			time -= bulletSpawnPeriod;

			spawnBullets(context);
		}
	}

	mat4 Enemy::getModelTransform() const {
		mat4 model(1.0f);
		model = glm::translate(model, pos);
		return model;
	}


	constexpr float ROTATE_PER_SEC = glm::radians(45.0f);
	constexpr float BULLET_SPEED = 0.15f;
	constexpr vec2 ANGLE_NORMAL(0.0f, -1.0f);


	Enemy1::Enemy1(DrawContext& drawContext, DrawContext& bulletDrawContext, const glm::vec3& pos) noexcept:
				Enemy(drawContext, bulletDrawContext, pos, 0.5f) {}
	
	void Enemy1::tick(TickContext& context) {
		vec3 dir3d = context.player->getPos() - pos;
		vec2 dir(dir3d.x, dir3d.z);

		if (glm::length(dir) == 0) {
			Enemy::tick(context);
			return;
		}

		float targetAngle = glm::orientedAngle(ANGLE_NORMAL, glm::normalize(dir));
		angle += clamp(targetAngle - angle, -ROTATE_PER_SEC, ROTATE_PER_SEC);

		Enemy::tick(context);
	}

	void Enemy1::spawnBullets(TickContext& context) {
		vec2 velocity0 = ANGLE_NORMAL * BULLET_SPEED;

		for (int i = 0; i < 5; i++) {
			vec2 velocity = glm::rotate(velocity0, angle + glm::radians(-90.0f + i * 45));

			context.addEntity(make_shared<EnemyBullet>(
				bulletDrawContext, spawnUnbreakable, vec3(velocity.x, 0.0f, velocity.y), pos
			));
		}

		spawnUnbreakable = !spawnUnbreakable;
	}
}
