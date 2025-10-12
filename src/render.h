#ifndef HACK_GAME__RENDER_H
#define HACK_GAME__RENDER_H

#include "gl_fwd.h"
#include <map>

struct GLFWwindow;

namespace hack_game {
	class Initializer;
	class Shader;
	class TickContext;

	void renderGL(const Initializer&, const TickContext&, Shader& postprocessingShader, const std::map<GLuint, Shader*>& shaderById, std::ofstream& fpsLog, float endGameTime);
	void renderImGui(GLFWwindow*, float deltaTime, float endGameTime);
}

#endif