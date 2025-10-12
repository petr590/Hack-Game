#ifndef HACK_GAME__MAIN_H
#define HACK_GAME__MAIN_H

#include "init.h"
#include "context/shader.h"

namespace hack_game {
	class Initializer;
	void mainLoop(const Initializer&, Shader& postprocessingShader, TickContext& tickContext, const std::map<GLuint, Shader*>& shaderById);
	void onChangeWindowSize(int width, int height);
}

#endif