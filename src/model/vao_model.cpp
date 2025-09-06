#include "vao_model.h"
#include "context/draw_context.h"
#include "util.h"

#define GLEW_STATIC
#include <GL/glew.h>

namespace hack_game {
	using glm::vec3;

	VAOModel::VAOModel(uint32_t color) noexcept:
			color(colorAsVec3(color)) {}

	VAOModel::VAOModel(uint32_t color, const VAOModel& model):
			color(colorAsVec3(color)),
			vertexArray(model.vertexArray),
			indices(model.indices) {}

	void VAOModel::generateVertexArray() {
		vertexArray = getVertexArray();
	}

	void VAOModel::draw(Shader& shader) const {
		draw(shader, color);
	}

	void VAOModel::draw(Shader& shader, const vec3& color) const {
		shader.setModelColor(color);

		assert(vertexArray != 0);

		glBindVertexArray(vertexArray);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);
	}
}