#ifndef HACK_GAME_BULLET_H
#define HACK_GAME_BULLET_H

#include "entity.h"
#include "model.h"

namespace hack_game {

	class Bullet: public Entity, public std::enable_shared_from_this<Entity> {
		const Model& model;

		const float angle;
		const glm::vec3 velocity;
		glm::vec3 pos;

	public:
		Bullet(DrawContext& drawContext, const Model& model, float angle, const glm::vec3& velocity, const glm::vec3& pos):
				Entity(drawContext), model(model), angle(angle), velocity(velocity), pos(pos) {}
		
		void tick(TickContext&) override;
		void draw() const override;
	};
}

#endif