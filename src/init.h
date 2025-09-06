#ifndef HACK_GAME__INIT_H
#define HACK_GAME__INIT_H

#include "context/tick_context.h"

class GLFWwindow;

namespace hack_game {
	class DrawContext;

	extern int windowWidth, windowHeight, refreshRate;
	extern std::shared_ptr<Player> player;

	GLFWwindow* initGLFW();
	void initGL();
	
	TickContext createTickContext(DrawContext&);
	void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
}

#endif