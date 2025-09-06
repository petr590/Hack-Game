#ifndef HACK_GAME__ENTITY_ANIMATION__ANIMATION_H
#define HACK_GAME__ENTITY_ANIMATION__ANIMATION_H

#include "entity/simple_entity.h"
#include <memory>

namespace hack_game {

	class Animation: public SimpleEntity, public std::enable_shared_from_this<Entity> {
	protected:
		const float size;
		const float duration;
		float time = 0;
		glm::vec3 pos;
		glm::mat3 viewMat3;

	public:
		Animation(Shader&, float size, float duration) noexcept;

		float getTime() const noexcept {
			return time;
		}

		bool isFinished() const noexcept {
			return time >= duration;
		}

		bool isTransparent() const noexcept override {
			return true;
		}

		void tick(TickContext&) override;
		void draw() const override;
		glm::mat4 getModelTransform() const override;
	};
}

#endif