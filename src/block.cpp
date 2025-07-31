#include "block.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace hack_game {
	void Block::draw(GLuint vertexColorLocation) const {
		glm::mat4 model(1.0f);
		model = glm::scale(model, glm::vec3(0.2f));
		model = glm::translate(model, glm::vec3(SIZE * pos.x, 0, SIZE * pos.y));
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

		this->model.draw(vertexColorLocation);
	}
}