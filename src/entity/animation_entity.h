#ifndef HACK_GAME__ENTITY__ANIMATION_ENTITY_H
#define HACK_GAME__ENTITY__ANIMATION_ENTITY_H

#include "simple_entity.h"

namespace hack_game {

	class AnimationEntity: public SimpleEntity {
		glm::vec3 pos;

	public:
		explicit AnimationEntity(DrawContext& drawContext) noexcept;

		void tick(TickContext&) override;
		glm::mat4 getModelTransform() const override;
	};
}

#endif
