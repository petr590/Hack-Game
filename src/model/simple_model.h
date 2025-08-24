#ifndef HACK_GAME__MODEL__SIMPLE_MODEL_H
#define HACK_GAME__MODEL__SIMPLE_MODEL_H

#include "model.h"
#include <string>

namespace hack_game {

	class SimpleModel: public Model {
		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;
		GLuint vertexArray = 0;
	
	public:
		const glm::vec3 color;

		SimpleModel(GLuint color, const std::string& path);
		SimpleModel(GLuint color, const SimpleModel&);

		void generateVertexArray() override;
		void draw(DrawContext&) const override;
		void draw(DrawContext&, const glm::vec3& color) const override;
	};
}

#endif
