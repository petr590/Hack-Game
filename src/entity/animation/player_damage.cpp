#include "player_damage.h"
#include "context/tick_context.h"
#include "context/draw_context.h"

namespace hack_game {

	static const float DURATION = 1.3f;
	static const float SIZE     = TILE_SIZE * 5.5f;
	static const float Y_OFFSET = TILE_SIZE * 0.05f;

	PlayerDamageAnimation::PlayerDamageAnimation(std::shared_ptr<const EntityWithPos>&& entity, DrawContext& context):
			Animation(std::move(entity), context.getShader("playerDamage"), DURATION, SIZE, Y_OFFSET) {}
}