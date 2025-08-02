#include "block.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace hack_game {
	void Block::draw(GLuint modelColorLocation) const {
		glm::mat4 model(1.0f);

		model = glm::translate(model, glm::vec3(
			SIZE * (pos.x + 0.5f),
			0.0f,
			SIZE * (pos.y + 0.5f)
		));

		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

		this->model.draw(modelColorLocation);
	}
}