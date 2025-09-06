#ifndef HACK_GAME__MODEL__VAO_MODEL_H
#define HACK_GAME__MODEL__VAO_MODEL_H

#include "model.h"

namespace hack_game {

	class VAOModel: public Model {
	protected:
		const glm::vec3 color;
		GLuint vertexArray = 0;
		std::vector<GLuint> indices;
	
	public:
		VAOModel(uint32_t color) noexcept;
		VAOModel(uint32_t color, const VAOModel&);

		const glm::vec3& getColor() const noexcept {
			return color;
		}

		void generateVertexArray() override;
		void draw(Shader&) const override;
		void draw(Shader&, const glm::vec3& color) const override;
	
	protected:
		virtual GLuint getVertexArray() const = 0;
	};
}

#endif