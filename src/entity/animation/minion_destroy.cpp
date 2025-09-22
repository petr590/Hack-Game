#include "minion_destroy.h"
#include "context/draw_context.h"

namespace hack_game {

	static const float SIZE = 5;
	static const float DURATION = 0.35f * 5;

	MinionDestroyAnimation::MinionDestroyAnimation(std::shared_ptr<const EntityWithPos>&& entity, DrawContext& drawContext) noexcept:
			BillboardAnimation(std::move(entity), drawContext.shaders.at("minionDestroy"), SIZE, DURATION) {}
}