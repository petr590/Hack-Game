#include "enemy_damage.h"
#include "context/draw_context.h"
#include "context/tick_context.h"

namespace hack_game {

	static const float SIZE = TILE_SIZE * 2;
	static const float DURATION = 0.25f;

	EnemyDamageAnimation::EnemyDamageAnimation(std::shared_ptr<const EntityWithPos>&& entity, DrawContext& drawContext) noexcept:
			BillboardAnimation(std::move(entity), drawContext.shaders.at("enemyDamage"), SIZE, DURATION) {}
}