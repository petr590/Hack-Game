#ifndef HACK_GAME__INIT_H
#define HACK_GAME__INIT_H

#include "context/tick_context.h"

class GLFWwindow;

namespace hack_game {
	class DrawContext;

	extern int windowWidth, windowHeight, refreshRate;

	struct FramebufferInfo {
		GLuint msFramebuffer;
		GLuint msTexture;
		GLuint msRenderbuffer;

		GLuint framebuffer;
		GLuint texture;
	};

	GLFWwindow* initGLFW();
	FramebufferInfo initGL();
	void changeFramebufferSize(const FramebufferInfo&);
	
	TickContext createTickContext(DrawContext&);
	void keyCallback(GLFWwindow*, int key, int scancode, int action, int mode);
	void framebufferSizeCallback(GLFWwindow*, int width, int height);
}

#endif