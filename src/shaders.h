#ifndef HACK_GAME_SHADERS_H
#define HACK_GAME_SHADERS_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace hack_game {
	extern GLuint width, height;

	GLuint compileShader(GLenum type, const char* filename);
	GLuint createShaderProgram(const char* vertexShaderPath, const char* fragmentShaderPath, const char* geometryShaderPath = nullptr);
	void initShaderUniforms(GLuint mainShaderProgram, GLuint lightShaderProgram);
}

#endif