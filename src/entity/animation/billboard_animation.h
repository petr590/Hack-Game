#ifndef HACK_GAME__ENTITY__ANIMATION__BILLBOARD_ANIMATION_H
#define HACK_GAME__ENTITY__ANIMATION__BILLBOARD_ANIMATION_H

#include "animation.h"

namespace hack_game {

	class BillboardAnimation: public Animation {
		glm::mat4 view;
		
	public:
		using Animation::Animation;

		const glm::mat4& getView() const noexcept {
			return view;
		}
		
		void tick(TickContext&) override;
		glm::mat4 getModelTransform() const override;
	};
}

#endif