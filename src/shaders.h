#ifndef HACK_GAME__SHADERS_H
#define HACK_GAME__SHADERS_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/vec3.hpp>

namespace hack_game {
	class DrawContext;

	extern int windowWidth, windowHeight;

	constexpr glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
	constexpr glm::vec3 lightPos(0.5f, 0.6f, 0.5f);

	GLuint createShaderProgram(const char* vertexShaderPath, const char* fragmentShaderPath);
	GLuint createAnimationShaderProgram(const char* vertexShaderName, const char* fragmentShaderName);
	void initShaderUniforms(DrawContext&);
}

#endif
