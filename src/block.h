#ifndef HACK_GAME_BLOCK_H
#define HACK_GAME_BLOCK_H

#include "entity.h"
#include "model.h"
#include <glm/glm.hpp>

namespace hack_game {

	class Block: public Entity {
	public:
		const glm::ivec2 pos;

	private:
		const Model& model;

	public:
		Block(DrawContext& drawContext, glm::ivec2 pos, const Model& model):
				Entity(drawContext), pos(pos), model(model) {}
		
		void tick(TickContext&) override;
		void draw() const override;
	};
}

#endif