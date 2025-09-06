#include "enemy_damage.h"
#include "context/draw_context.h"
#include "context/tick_context.h"

namespace hack_game {

	static const float SIZE = TILE_SIZE * 2;
	static const float DURATION = 0.25f;

	EnemyDamageAnimation::EnemyDamageAnimation(DrawContext& drawContext) noexcept:
			Animation(drawContext.shaders["enemyDamage"], SIZE, DURATION) {}
}