#ifndef HACK_GAME__MODEL__LINE_MODEL_H
#define HACK_GAME__MODEL__LINE_MODEL_H

#include "vao_model.h"
#include <string>

namespace hack_game {

	class FrameModel: public VAOModel {
		const glm::vec3 color;
		std::vector<glm::vec3> vertices;

	public:
		FrameModel(uint32_t color, const char* relativePath);

		void draw(Shader&) const override;
	
	protected:
		GLuint createVertexArray() override;
	};
}

#endif