#ifndef HACK_GAME__ENTITY_ANIMATION__ENEMY_DAMAGE_H
#define HACK_GAME__ENTITY_ANIMATION__ENEMY_DAMAGE_H

#include "billboard_animation.h"

namespace hack_game {

	class EnemyDamageAnimation: public BillboardAnimation {
	public:
		EnemyDamageAnimation(std::shared_ptr<const EntityWithPos>&&, DrawContext&) noexcept;
	};
}

#endif