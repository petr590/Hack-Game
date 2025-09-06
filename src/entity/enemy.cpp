#include "enemy.h"
#include "player.h"
#include "bullet.h"
#include "animation/enemy_damage.h"
#include "animation/enemy_destroy.h"
#include "model/models.h"
#include "context/draw_context.h"
#include "context/tick_context.h"
#include "globals.h"
#include "util.h"

#include <glm/gtx/vector_angle.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace hack_game {
	using std::isnan;
	using std::clamp;
	using std::make_shared;

	using glm::vec2;
	using glm::vec3;
	using glm::mat4;

	#define MODEL models::sphere

	Enemy::Enemy(DrawContext& drawContext, float bulletSpawnPeriod, const glm::vec3& pos) noexcept:
			SimpleEntity(drawContext.mainShader, MODEL),
			Damageable(Side::ENEMY, 7),
			drawContext(drawContext),
			bulletSpawnPeriod(bulletSpawnPeriod),
			pos(pos) {}


	bool Enemy::hasCollision(const vec3& point) const {
		return isPointInsideSphere(point, pos, RADIUS);
	}


	static const float BRIGHT_DURATION = 0.04f;

	void Enemy::damage(TickContext& context, hp_t damage) {
		Damageable::damage(context, damage);

		if (destroyed()) {
			won = true;

			animation = make_shared<EnemyDestroyAnimation>(drawContext);
			context.addEntity(animation);
			context.removeEntity(shared_from_this());

		} else if (animation == nullptr || !animation->isFinished()) {

			animation = make_shared<EnemyDamageAnimation>(drawContext);
			context.addEntity(animation);
		}
	}

	void Enemy::tick(TickContext& context) {
		time += context.deltaTime;

		if (time >= bulletSpawnPeriod) {
			time -= bulletSpawnPeriod;

			spawnBullets(context);
		}
	}

	void Enemy::draw() const {
		shader.setModel(getModelTransform());
		
		if (animation != nullptr && animation->getTime() <= BRIGHT_DURATION) {
			model.draw(shader, MODEL.getColor() * 1.5f);
		} else {
			model.draw(shader);
		}

		SimpleEntity::draw();
	}

	mat4 Enemy::getModelTransform() const {
		return glm::translate(mat4(1.0f), pos);
	}


	static const float ROTATE_PER_SEC = glm::radians(45.0f);

	Enemy1::Enemy1(DrawContext& drawContext, const glm::vec3& pos) noexcept:
			Enemy(drawContext, 0.5f, pos) {}
	
	
	void Enemy1::tick(TickContext& context) {
		float targetAngle = horizontalAngleBetween(pos, context.player->getPos());

		if (!isnan(targetAngle)) {
			angle += clamp(targetAngle - angle, -ROTATE_PER_SEC, ROTATE_PER_SEC);
		}

		Enemy::tick(context);
	}


	void Enemy1::spawnBullets(TickContext& context) {
		vec2 velocity0 = ANGLE_NORMAL * EnemyBullet::DEFAULT_SPEED;

		for (int i = 0; i < 5; i++) {
			vec2 velocity = glm::rotate(velocity0, angle + glm::radians(-90.0f + i * 45));

			context.addEntity(make_shared<EnemyBullet>(
				drawContext.lightShader, spawnUnbreakable, vec3(velocity.x, 0.0f, velocity.y), pos
			));
		}

		spawnUnbreakable = !spawnUnbreakable;
	}
}
