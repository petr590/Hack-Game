#ifndef HACK_GAME__MODEL__POSTPROCESSING_MODEL_H
#define HACK_GAME__MODEL__POSTPROCESSING_MODEL_H

#include "vao_model.h"
#include <glm/vec2.hpp>

namespace hack_game {

	class PostprocessingModel: public VAOModel {
		struct Vertex {
			glm::vec2 pos;
			glm::vec2 texCoord;
		};

		std::vector<Vertex> vertices;
	
	public:
		PostprocessingModel();

	protected:
		GLuint createVertexArray() override;
	};
}

#endif