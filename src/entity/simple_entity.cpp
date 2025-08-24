#include "simple_entity.h"
#include "../model/model.h"
#include "../context/draw_context.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

namespace hack_game {
	GLuint SimpleEntity::getShaderProgram() const {
		return drawContext.shaderProgram;
	}

	void SimpleEntity::draw() const {
		glUseProgram(drawContext.shaderProgram);

		glm::mat4 modelMat = getModelTransform();
		glUniformMatrix4fv(drawContext.modelUniform, 1, GL_FALSE, glm::value_ptr(modelMat));

		model.draw(drawContext);
	}

	glm::mat4 SimpleEntity::getModelTransform() const {
		return glm::mat4(1.0f);
	}
}
