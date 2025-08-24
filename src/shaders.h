#ifndef HACK_GAME__SHADERS_H
#define HACK_GAME__SHADERS_H

#include <initializer_list>

#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/vec3.hpp>

namespace hack_game {
	extern int width, height;

	constexpr glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
	constexpr glm::vec3 lightPos(0.5f, 0.6f, 0.5f);

	GLuint compileShader(GLenum type, const char* filename);
	GLuint createShaderProgram(const char* vertexShaderPath, const char* fragmentShaderPath, const char* geometryShaderPath = nullptr);
	void initShaderUniforms(GLuint mainShader, std::initializer_list<GLuint> otherShaders);
}

#endif
