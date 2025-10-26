#ifndef HACK_GAME__MODEL__POSTPROCESSING_MODEL_H
#define HACK_GAME__MODEL__POSTPROCESSING_MODEL_H

#include "vao_model.h"
#include <glm/vec2.hpp>

namespace hack_game {

	class PostprocessingModel: public VAOModel {
	public:
		struct Vertex;
	
	private:
		std::vector<Vertex> vertices;
	
	public:
		PostprocessingModel();
		~PostprocessingModel();

	protected:
		GLuint createVertexArray() override;
	};
}

#endif