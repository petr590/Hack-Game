#include "minion_destroy.h"
#include "context/draw_context.h"

namespace hack_game {

	static const float SIZE = 5;
	static const float DURATION = 0.35f * 10;

	MinionDestroyAnimation::MinionDestroyAnimation(DrawContext& drawContext) noexcept:
			Animation(drawContext.shaders["minionDestroy"], SIZE, DURATION) {}
}