#ifndef HACK_GAME__ENTITY__SHOOTER_H
#define HACK_GAME__ENTITY__SHOOTER_H

#include "simple_entity.h"
#include "damageable.h"

namespace hack_game {

	class Minion: public SimpleEntity, public Damageable {
		Shader& bulletShader;
		glm::vec3 pos;
		float angle = 0;
		float time = 0;

	public:
		Minion(DrawContext&, const glm::vec3& pos) noexcept;

		void tick(TickContext&) override;
		glm::mat4 getModelTransform() const override;
		bool hasCollision(const glm::vec3& point) const override;
	};
}

#endif