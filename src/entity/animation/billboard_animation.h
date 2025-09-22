#ifndef HACK_GAME__ENTITY_ANIMATION__BILLBOARD_ANIMATION_H
#define HACK_GAME__ENTITY_ANIMATION__BILLBOARD_ANIMATION_H

#include "animation.h"

namespace hack_game {

	class BillboardAnimation: public Animation {
		glm::mat3 viewMat3;
		
	public:
		BillboardAnimation(std::shared_ptr<const EntityWithPos>&&, Shader&, Model&, float size, float duration) noexcept;
		BillboardAnimation(std::shared_ptr<const EntityWithPos>&&, Shader&, float size, float duration) noexcept;
		
		void tick(TickContext&) override;
		glm::mat4 getModelTransform() const override;
	};
}

#endif