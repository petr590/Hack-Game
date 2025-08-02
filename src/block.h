#ifndef HACK_GAME_BLOCK_H
#define HACK_GAME_BLOCK_H

#include "model.h"
#include <glm/glm.hpp>

namespace hack_game {

	class Block {
	public:
		const glm::ivec2 pos;

	private:
		const Model& model;
		const GLuint shaderProgram;
		const GLuint modelLocation;

	public:
		static constexpr GLfloat SIZE = 0.05f;
		static constexpr GLfloat EDGE = 0.04f;
		
		Block(glm::ivec2 pos, const Model& model, GLuint shaderProgram, GLuint modelLocation):
				pos(pos), model(model), shaderProgram(shaderProgram), modelLocation(modelLocation) {}
		
		void draw(GLuint modelColorLocation) const;
	};
}

#endif