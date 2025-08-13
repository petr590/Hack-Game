#include "simple_entity.h"

#include <glm/gtc/type_ptr.hpp>

namespace hack_game {
	void SimpleEntity::draw() const {
		glUseProgram(drawContext.shaderProgram);

		glm::mat4 modelMat = getModelTransform();
		glUniformMatrix4fv(drawContext.modelLocation, 1, GL_FALSE, glm::value_ptr(modelMat));

		model.draw(drawContext);
	}

	glm::mat4 SimpleEntity::getModelTransform() const {
		return glm::mat4(1.0f);
	}
}