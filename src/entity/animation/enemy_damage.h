#ifndef HACK_GAME__ENTITY_ANIMATION__ENEMY_DAMAGE_H
#define HACK_GAME__ENTITY_ANIMATION__ENEMY_DAMAGE_H

#include "animation.h"

namespace hack_game {

	class EnemyDamageAnimation: public Animation {
	public:
		explicit EnemyDamageAnimation(DrawContext&) noexcept;
	};
}

#endif