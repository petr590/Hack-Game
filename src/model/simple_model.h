#ifndef HACK_GAME__MODEL__SIMPLE_MODEL_H
#define HACK_GAME__MODEL__SIMPLE_MODEL_H

#include "vao_model.h"
#include <string>

namespace hack_game {

	class SimpleModel: public VAOModel {
		struct Vertex {
			const glm::vec3 pos;
			const glm::vec3 normal;

			constexpr Vertex(const glm::vec3 pos, const glm::vec3 normal) noexcept:
				pos(pos), normal(normal) {}
		};


		std::vector<Vertex> vertices;
	
	public:
		SimpleModel(uint32_t color, const std::string& path);
		SimpleModel(uint32_t color, const SimpleModel&);

		GLuint getVertexArray() const override;
	};
}

#endif