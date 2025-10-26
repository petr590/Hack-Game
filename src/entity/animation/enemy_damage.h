#ifndef HACK_GAME__ENTITY__ANIMATION__ENEMY_DAMAGE_H
#define HACK_GAME__ENTITY__ANIMATION__ENEMY_DAMAGE_H

#include "billboard_animation.h"

namespace hack_game {

	class EnemyDamageAnimation: public BillboardAnimation {
	public:
		EnemyDamageAnimation(std::shared_ptr<const EntityWithPos>&&, ShaderManager&) noexcept;
	};
}

#endif