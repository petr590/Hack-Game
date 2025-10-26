#ifndef HACK_GAME__ENTITY__PLATFORM_H
#define HACK_GAME__ENTITY__PLATFORM_H

#include "simple_entity.h"

namespace hack_game {

	class Platform: public SimpleEntity {
		const glm::vec2 scale;

	public:
		Platform(Shader& shader, const glm::vec2& scale) noexcept;
		
		glm::mat4 getModelTransform() const override;
	};
}

#endif