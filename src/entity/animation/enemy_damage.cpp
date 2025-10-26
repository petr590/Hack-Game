#include "enemy_damage.h"
#include "entity/enemy.h"
#include "shader/shader_manager.h"
#include "level/level.h"

namespace hack_game {

	static const float DURATION = 0.25f;
	static const float SIZE = TILE_SIZE * 2;

	EnemyDamageAnimation::EnemyDamageAnimation(std::shared_ptr<const EntityWithPos>&& entity, ShaderManager& shaderManager) noexcept:
			BillboardAnimation(std::move(entity), shaderManager.getShader("enemyDamage"), DURATION, SIZE, Enemy::RADIUS) {}
}