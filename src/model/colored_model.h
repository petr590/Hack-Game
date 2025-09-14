#ifndef HACK_GAME__MODEL__SIMPLE_MODEL_H
#define HACK_GAME__MODEL__SIMPLE_MODEL_H

#include "vao_model.h"
#include <string>

namespace hack_game {

	class ColoredModel: public VAOModel {
		struct Vertex {
			const glm::vec3 pos;
			const glm::vec3 normal;

			constexpr Vertex(const glm::vec3 pos, const glm::vec3 normal) noexcept:
				pos(pos), normal(normal) {}
		};


		const glm::vec3 color;
		std::vector<Vertex> vertices;
	
	public:
		ColoredModel(uint32_t color, const char* relativePath);
		ColoredModel(uint32_t color, const ColoredModel&);

		const glm::vec3& getColor() const noexcept {
			return color;
		}

		GLuint createVertexArray() override;

		void draw(Shader&) const override;
		void draw(Shader&, const glm::vec3& color) const;
	};
}

#endif