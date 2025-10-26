#include "damageable.h"
#include "level/level.h"

namespace hack_game {

	using glm::vec3;

	Damageable::Damageable(Side side, hp_t hitpoints) noexcept:
			side(side), hitpoints(hitpoints) {}

	
	void Damageable::damage(Level& level, hp_t damage) {
		if (invulnerable()) {
			return;
		}

		const hp_t oldHitpoints = hitpoints;
		hitpoints -= damage;

		if (oldHitpoints > 0 && hitpoints <= 0) {
			onDestroy(level);
		}
	}

	void Damageable::onDestroy(Level& level) {
		level.removeEntity(shared_from_this());
	}
}