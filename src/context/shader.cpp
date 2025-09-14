#include "shader.h"
#include <stdexcept>
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

namespace hack_game {
	Shader::Shader() noexcept:
			id(0),
			model      (-1),
			view       (-1),
			modelColor (-1),
			centerPos  (-1),
			progress   (-1),
			mode       (-1),
			seed       (-1),
			texture0   (-1),
			texture1   (-1) {}

	Shader::Shader(GLuint shaderProgram):
			id(shaderProgram),
			model      (glGetUniformLocation(shaderProgram, "model")),
			view       (glGetUniformLocation(shaderProgram, "view")),
			modelColor (glGetUniformLocation(shaderProgram, "modelColor")),
			centerPos  (glGetUniformLocation(shaderProgram, "centerPos")),
			progress   (glGetUniformLocation(shaderProgram, "progress")),
			mode       (glGetUniformLocation(shaderProgram, "mode")),
			seed       (glGetUniformLocation(shaderProgram, "seed")),
			texture0   (glGetUniformLocation(shaderProgram, "texture0")),
			texture1   (glGetUniformLocation(shaderProgram, "texture1")) {
		
		if (texture0 >= 0 || texture1 >= 0) {
			glUseProgram(id);
			setTexture0(0);
			setTexture1(1);
		}
	}
	

	void Shader::setModel(const glm::mat4& mat) {
		if (model >= 0) {
			glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(mat));
		}
	}

	void Shader::setView(const glm::mat4& mat) {
		if (view >= 0) {
			glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(mat));
		}
	}

	void Shader::setModelColor(const glm::vec3& vec) {
		if (modelColor >= 0) {
			glUniform3fv(modelColor, 1, glm::value_ptr(vec));
		}
	}

	void Shader::setCenterPos(const glm::vec3& vec) {
		if (centerPos >= 0) {
			glUniform3fv(centerPos, 1, glm::value_ptr(vec));
		}
	}

	void Shader::setProgress(float value) {
		if (progress >= 0) {
			glUniform1f(progress, value);
		}
	}

	// Remove if unused
	void Shader::setMode(GLuint value) {
		if (mode >= 0) {
			glUniform1ui(mode, value);
		}
	}

	void Shader::setSeed(GLint value) {
		if (seed >= 0) {
			glUniform1i(seed, value);
		}
	}

	void Shader::setTexture0(GLint value) {
		if (texture0 >= 0) {
			glUniform1i(texture1, value);
		}
	}

	void Shader::setTexture1(GLint value) {
		if (texture1 >= 0) {
			glUniform1i(texture1, value);
		}
	}
}
