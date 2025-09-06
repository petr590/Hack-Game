#ifndef HACK_GAME__MODEL__LINE_MODEL_H
#define HACK_GAME__MODEL__LINE_MODEL_H

#include "vao_model.h"
#include <string>

namespace hack_game {

	class FrameModel: public VAOModel {
		std::vector<glm::vec3> vertices;

	public:
		FrameModel(uint32_t color, const std::string& path);

		GLuint getVertexArray() const override;

		void draw(Shader&, const glm::vec3&) const override;
	};
}

#endif