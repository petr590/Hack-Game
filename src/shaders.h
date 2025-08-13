#ifndef HACK_GAME_SHADERS_H
#define HACK_GAME_SHADERS_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>

namespace hack_game {
	extern GLuint width, height;

	constexpr glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
	constexpr glm::vec3 lightPos(0.5f, 0.6f, 0.5f);

	GLuint compileShader(GLenum type, const char* filename);
	GLuint createShaderProgram(const char* vertexShaderPath, const char* fragmentShaderPath, const char* geometryShaderPath = nullptr);
	void initShaderUniforms(GLuint mainShaderProgram, GLuint lightShaderProgram);
}

#endif