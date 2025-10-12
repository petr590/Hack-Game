#ifndef HACK_GAME__ENTITY__ANIMATION__PLAYER_DAMAGE_H
#define HACK_GAME__ENTITY__ANIMATION__PLAYER_DAMAGE_H

#include "animation.h"

namespace hack_game {

	class PlayerDamageAnimation: public Animation {
	public:
		PlayerDamageAnimation(std::shared_ptr<const EntityWithPos>&&, DrawContext&);
	};
}

#endif