#ifndef HACK_GAME__ENTITY_ANIMATION__ANIMATION_H
#define HACK_GAME__ENTITY_ANIMATION__ANIMATION_H

#include "entity/simple_entity.h"
#include <memory>

namespace hack_game {

	class EntityWithPos;

	class Animation: public SimpleEntity, public std::enable_shared_from_this<Entity> {
	protected:
		const std::shared_ptr<const EntityWithPos> entity;
		const float size;
		const float duration;
		float time = 0;
		glm::vec3 pos;

	public:
		Animation(std::shared_ptr<const EntityWithPos>&&, Shader&, Model&, float size, float duration) noexcept;
		Animation(std::shared_ptr<const EntityWithPos>&&, Shader&, float size, float duration) noexcept;

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
	
	protected:
		virtual void onRemove() {}
	};
}

#endif