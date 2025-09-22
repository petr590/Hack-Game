#ifndef HACK_GAME__ENTITY_ANIMATION__MINION_DESTROY_H
#define HACK_GAME__ENTITY_ANIMATION__MINION_DESTROY_H

#include "billboard_animation.h"

namespace hack_game {

	class MinionDestroyAnimation: public BillboardAnimation {
	public:
		MinionDestroyAnimation(std::shared_ptr<const EntityWithPos>&&, DrawContext&) noexcept;
	};
}

#endif