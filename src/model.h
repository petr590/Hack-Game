#ifndef HACK_GAME_MODEL_H
#define HACK_GAME_MODEL_H

#include "vertex.h"
#include "entity.h"
#include <string>
#include <vector>
#include <cassert>

namespace hack_game {
	
	class Model {
		const GLuint color;
		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;
		GLuint vertexArray = 0;
	
	public:
		Model(GLuint color, const std::string& path);

		Model(const Model&) = delete;
		Model& operator=(const Model&) = delete;

		void generateVertexArray();
		void draw(DrawContext&) const;
	};
}

#endif