#ifndef HACK_GAME__MAIN_H
#define HACK_GAME__MAIN_H

#include "init.h"
#include "context/shader.h"

namespace hack_game {
	void mainLoop(GLFWwindow* const, const FramebufferInfo&, Shader& postprocessingShader, TickContext& tickContext, const std::map<GLuint, Shader*>& shaderById);
}

#endif