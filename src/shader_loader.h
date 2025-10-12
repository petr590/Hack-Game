#ifndef HACK_GAME__SHADERS_H
#define HACK_GAME__SHADERS_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/vec3.hpp>

namespace hack_game {
	class DrawContext;

	GLuint createShaderProgram(const char* vertexShaderPath, const char* fragmentShaderPath);
	GLuint createAnimationShaderProgram(const char* vertexShaderName, const char* fragmentShaderName);
	void onShadersLoaded();
}

#endif
