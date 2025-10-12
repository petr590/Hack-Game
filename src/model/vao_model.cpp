#include "vao_model.h"
#include "util.h"

#define GLEW_STATIC
#include <GL/glew.h>

namespace hack_game {
	using glm::vec3;

	VAOModel::VAOModel() noexcept {}

	VAOModel::VAOModel(const VAOModel& model):
			indices(model.indices),
			vertexArray(model.vertexArray) {}

	void VAOModel::generateVertexArray() {
		vertexArray = createVertexArray();
	}

	void VAOModel::draw(Shader&) const {
		assert(vertexArray != 0);

		glBindVertexArray(vertexArray);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);
	}
}