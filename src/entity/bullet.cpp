#include "bullet.h"
#include "enemy.h"
#include "player.h"
#include "block.h"
#include "../model/models.h"
#include "../context/draw_context.h"
#include "../context/tick_context.h"

#include <glm/gtc/matrix_transform.hpp>

namespace hack_game {
	using std::min;
	using std::max;
	using std::clamp;
	using std::isinf;

	using glm::uvec2;
	using glm::vec2;
	using glm::vec3;
	using glm::mat4;

	static const float LIMIT = 5.0f;
	static const float ENEMY_BULLET_RADIUS = Enemy::RADIUS;
	static const float PLAYER_BULLET_RADIUS = 0.005657f;


	Bullet::Bullet(
		DrawContext& drawContext,
		const Model& model,
		float angle,
		const glm::vec3& velocity,
		const glm::vec3& pos
	) noexcept:
			SimpleEntity(drawContext, model),
			angle(angle),
			velocity(velocity),
			pos(pos) {}

	void Bullet::tick(TickContext& context) {
		pos += velocity * context.deltaTime;

		if (checkCollision(context)) {
			context.removeEntity(shared_from_this());
		}

		if (pos.x > LIMIT || pos.x < -LIMIT ||
			pos.z > LIMIT || pos.z < -LIMIT) {

			context.removeEntity(shared_from_this());
		}
	}

	mat4 Bullet::getModelTransform() const {
		mat4 model(1.0f);
		model = glm::translate(model, pos);
		model = glm::rotate(model, angle, vec3(0.0f, 1.0f, 0.0f));
		return model;
	}





	static Block* getCollisionWithBlock(TickContext& context, const vec2& pos2d) {
		const uvec2 mapPos = context.getMapPos(pos2d);

		if (context.map[mapPos] == nullptr) {
			return nullptr;
		}

		const AABB block = context.map[mapPos]->getHitbox();

		if (block.containsInclusive(pos2d)) {
			return context.map[mapPos];
		}

		return nullptr;
	}

	static bool hasCollision(const vec3& pos1, const vec3& pos2) {
		vec3 diff = pos1 - pos2;
		return glm::dot(diff, diff) <= ENEMY_BULLET_RADIUS * ENEMY_BULLET_RADIUS;
	}


	// ---------------------------------------- PlayerBullet ----------------------------------------

	PlayerBullet::PlayerBullet(DrawContext& drawContext, float angle, const glm::vec3& velocity, const glm::vec3& pos):
			Bullet(drawContext, models::playerBullet, angle, velocity, pos) {}
	

	bool PlayerBullet::checkCollision(TickContext& context) {
		Block* block = getCollisionWithBlock(context, vec2(pos.x, pos.z));

		if (block != nullptr) {
			block->damage(context, 1.0f);
			return true;
		}

		for (const auto& bullet : context.getBreakableEnemyBullets()) {
			if (hasCollision(pos, bullet->getPos())) {
				context.removeEntity(bullet);
				return true;
			}
		}

		if (hasCollision(pos, context.enemy->getPos())) {
			context.enemy->damage(context, 1.0f);
			return true;
		}

		return false;
	}


	// ---------------------------------------- EnemyBullet ----------------------------------------

	EnemyBullet::EnemyBullet(DrawContext& drawContext, bool unbreakable, const glm::vec3& velocity, const glm::vec3& pos):
			Bullet(
				drawContext,
				unbreakable ? models::unbreakableSphere : models::breakableSphere,
				0.0f, velocity, pos
			),
			unbreakable(unbreakable) {}
	
	
	bool EnemyBullet::checkCollision(TickContext& context) {
		Block* block = getCollisionWithBlock(context, vec2(pos.x, pos.z));
		if (block != nullptr) {
			return true;
		}

		if (hasCollision(pos, context.player->getPos())) {
			context.player->damage(context, 1);
			return true;
		}

		return false;
	}
}
