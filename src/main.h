#ifndef HACK_GAME__MAIN_H
#define HACK_GAME__MAIN_H

#include "init.h"

namespace hack_game {
	// class WindowData;
	// void mainLoop(const WindowData&, ShaderManager&, Level&, const std::map<GLuint, Shader*>& shaderById);
	// void onChangeWindowSize(int width, int height);

	class WindowData;
	void mainLoop(const WindowData&, ShaderManager&, bool profile);
	void onChangeWindowSize(GLint width, GLint height);
}

#endif