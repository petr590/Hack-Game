#ifndef HACK_GAME_BLOCK_H
#define HACK_GAME_BLOCK_H

#include "model.h"
#include <glm/glm.hpp>

namespace hack_game {

	class Block {
		const bool solid;
		const glm::ivec2 pos;

		const Model& model;
		const GLuint shaderProgram;
		const GLuint modelLocation;

	public:
		static constexpr GLfloat SIZE = 0.05f;
		static constexpr GLfloat EDGE = 0.04f;
		
		Block(bool solid, glm::ivec2 pos, const Model& model, GLuint shaderProgram, GLuint modelLocation):
				solid(solid), pos(pos), model(model), shaderProgram(shaderProgram), modelLocation(modelLocation) {}
		
		void draw(GLuint vertexColorLocation) const;
	};
}

#endif