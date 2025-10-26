#include "simple_entity.h"
#include "model/model.h"
#include "shader/shader.h"

#include <glm/gtc/type_ptr.hpp>

namespace hack_game {
	GLuint SimpleEntity::getShaderProgram() const noexcept {
		return shader.getId();
	}

	void SimpleEntity::draw() const {
		shader.setModel(getModelTransform());
		model.draw(shader);
	}

	glm::mat4 SimpleEntity::getModelTransform() const {
		return glm::mat4(1.0f);
	}
}
