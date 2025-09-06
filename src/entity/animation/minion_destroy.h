#ifndef HACK_GAME__ENTITY_ANIMATION__MINION_DESTROY_H
#define HACK_GAME__ENTITY_ANIMATION__MINION_DESTROY_H

#include "animation.h"

namespace hack_game {

	class MinionDestroyAnimation: public Animation {
	public:
		explicit MinionDestroyAnimation(DrawContext&) noexcept;
	};
}

#endif