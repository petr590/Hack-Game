#include "damageable.h"
#include "context/tick_context.h"

namespace hack_game {

	using glm::vec3;

	Damageable::Damageable(Side side, hp_t hitpoints) noexcept:
			side(side), hitpoints(hitpoints) {}

	
	void Damageable::damage(TickContext& context, hp_t damage) {
		if (invulnerable()) {
			return;
		}

		const hp_t oldHitpoints = hitpoints;
		hitpoints -= damage;

		if (oldHitpoints > 0 && hitpoints <= 0) {
			onDestroy(context);
		}
	}

	void Damageable::onDestroy(TickContext& context) {
		context.removeEntity(shared_from_this());
	}
}