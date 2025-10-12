#ifndef HACK_GAME__ENTITY__SHOOTER_H
#define HACK_GAME__ENTITY__SHOOTER_H

#include "simple_entity.h"
#include "damageable.h"
#include "entity_with_pos.h"

namespace hack_game {

	class Minion: public SimpleEntity, public Damageable, public EntityWithPos {
		DrawContext& drawContext;
		glm::vec3 pos;
		float angle = 0;
		float time = 0;

	public:
		Minion(DrawContext&, const glm::vec3& pos) noexcept;

		const glm::vec3& getPos() const noexcept override {
			return pos;
		}

		std::shared_ptr<const Minion> shared_from_this() const;

		void tick(TickContext&) override;
		glm::mat4 getModelTransform() const override;
		bool hasCollision(const glm::vec3& point) const override;
	
	protected:
		void onDestroy(TickContext&) override;
	};
}

#endif