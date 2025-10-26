#ifndef HACK_GAME__ENTITY__ANIMATION__ANIMATION_H
#define HACK_GAME__ENTITY__ANIMATION__ANIMATION_H

#include "entity/simple_entity.h"
#include "model/models.h"
#include <memory>

namespace hack_game {

	class EntityWithPos;

	class Animation: public SimpleEntity, public std::enable_shared_from_this<Entity> {
	protected:
		const std::shared_ptr<const EntityWithPos> entity;
		const float size;
		const float duration;
		const float yOffset;
		float time = 0;

	public:
		Animation(std::shared_ptr<const EntityWithPos>&&, Shader&, float duration, float size, float yOffset, Model& = models::plane) noexcept;

		float getTime() const noexcept {
			return time;
		}

		bool isFinished() const noexcept {
			return time >= duration;
		}

		bool isTransparent() const noexcept override {
			return true;
		}

		void tick(Level&) override;
		void draw() const override;
		glm::mat4 getModelTransform() const override;
	
	protected:
		virtual void onRemove(Level&) {}
		glm::vec3 getPos() const noexcept;
	};
}

#endif