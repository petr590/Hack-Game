#include "shader.h"
#include <stdexcept>
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

namespace hack_game {
	using TextureArray = Shader::TextureArray;


	static constexpr TextureArray defaultTexureArray() {
		TextureArray arr {};

		for (size_t i = 0; i < arr.size(); i++) {
			arr[i] = -1;
		}

		return arr;
	}

	static TextureArray createTexureArray(GLuint id) {
		TextureArray arr {};
		
		static_assert(arr.size() < 10);
		char buffer[] = "texture0";

		for (size_t i = 0; i < arr.size(); i++) {
			buffer[sizeof(buffer) - 2] = '0' + i;
			arr[i] = glGetUniformLocation(id, buffer);
		}

		return arr;
	}
	

	Shader::Shader(const char* name) noexcept:
			id              (0),
			name            (name),
			model           (-1),
			view            (-1),
			modelColor      (-1),
			modelBrightness (-1),
			centerPos       (-1),
			pixelSize       (-1),
			progress        (-1),
			mode            (-1),
			seed            (-1),
			textures        (defaultTexureArray()) {}

	Shader::Shader(const char* name, GLuint shaderProgram):
			id              (shaderProgram),
			name            (name),
			model           (glGetUniformLocation(shaderProgram, "model")),
			view            (glGetUniformLocation(shaderProgram, "view")),
			modelColor      (glGetUniformLocation(shaderProgram, "modelColor")),
			modelBrightness (glGetUniformLocation(shaderProgram, "modelBrightness")),
			centerPos       (glGetUniformLocation(shaderProgram, "centerPos")),
			pixelSize       (glGetUniformLocation(shaderProgram, "pixelSize")),
			progress        (glGetUniformLocation(shaderProgram, "progress")),
			mode            (glGetUniformLocation(shaderProgram, "mode")),
			seed            (glGetUniformLocation(shaderProgram, "seed")),
			textures        (createTexureArray(id)) {

		glUseProgram(id);

		if (modelBrightness >= 0) {
			glUniform1f(modelBrightness, 1.0f);
		}

		for (size_t i = 0; i < textures.size(); i++) {
			if (textures[i] >= 0) glUniform1i(textures[i], i);
		}
	}


	template<class V, class F>
	static void setUniform(const char* uniformName, GLint uniformId, const char* name, const V& value, F func, bool printWarning = true) {
		if (uniformId >= 0) {
			func(uniformId, value);

		} else if (printWarning) {
			fprintf(stderr, "Warning: uniform %s is not found for shader %s\n", uniformName, name);
		}
	}
	

	void Shader::setModel(const glm::mat4& mat) {
		setUniform("model", model, name, mat, [] (GLint uid, const glm::mat4& val) { glUniformMatrix4fv(uid, 1, GL_FALSE, glm::value_ptr(val)); });
	}

	void Shader::setView(const glm::mat4& mat) {
		setUniform("view", view, name, mat, [] (GLint uid, const glm::mat4& val) { glUniformMatrix4fv(uid, 1, GL_FALSE, glm::value_ptr(val)); });
	}

	void Shader::setModelColor(const glm::vec3& vec) {
		setUniform("modelColor", modelColor, name, vec, [] (GLint uid, const glm::vec3& val) { glUniform3fv(uid, 1, glm::value_ptr(val)); }, false);
	}

	void Shader::setModelBrightness(float value) {
		setUniform("modelBrightness", modelBrightness, name, value, [] (GLint uid, float val) { glUniform1f(uid, val); });
	}

	void Shader::setCenterPos(const glm::vec3& vec) {
		setUniform("centerPos", centerPos, name, vec, [] (GLint uid, const glm::vec3& val) { glUniform3fv(uid, 1, glm::value_ptr(val)); });
	}

	void Shader::setPixelSize(const glm::vec2& vec) {
		setUniform("pixelSize", pixelSize, name, vec, [] (GLint uid, const glm::vec2& val) { glUniform2fv(uid, 1, glm::value_ptr(val)); });
	}

	void Shader::setProgress(float value) {
		setUniform("progress", progress, name, value, [] (GLint uid, float val) { glUniform1f(uid, val); });
	}

	void Shader::setMode(GLuint value) {
		setUniform("mode", mode, name, value, [] (GLint uid, GLuint val) { glUniform1ui(uid, val); });
	}

	void Shader::setSeed(GLint value) {
		setUniform("seed", seed, name, value, [] (GLint uid, GLint val) { glUniform1i(uid, val); });
	}
}
