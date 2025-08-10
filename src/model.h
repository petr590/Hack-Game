#ifndef HACK_GAME_MODEL_H
#define HACK_GAME_MODEL_H

#include "vertex.h"
#include "draw_context.h"
#define GLEW_STATIC
#include <GL/glew.h>
#include <string>
#include <vector>

namespace hack_game {
	
	class Model {
		static std::vector<Model*> models;

		const GLuint color;
		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;
		GLuint vertexArray = 0;
	
	public:
		static const std::vector<Model*>& getModels() noexcept {
			return models;
		}

		Model(GLuint color, const std::string& path);

		Model(const Model&) = delete;
		Model& operator=(const Model&) = delete;

		void generateVertexArray();
		void draw(DrawContext&) const;
	};
}

#endif