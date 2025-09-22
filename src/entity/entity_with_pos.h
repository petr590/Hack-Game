#ifndef HACK_GAME__ENTITY__ENTITY_WITH_POS_H
#define HACK_GAME__ENTITY__ENTITY_WITH_POS_H

#include "entity.h"

namespace hack_game {

	class EntityWithPos: public virtual Entity {
	public:
		virtual const glm::vec3& getPos() const noexcept = 0;
	};
}

#endif