#ifndef HACK_GAME_DRAW_CONTEXT_H
#define HACK_GAME_DRAW_CONTEXT_H

#define GLEW_STATIC
#include <GL/glew.h>

namespace hack_game {

	struct DrawContext {
		const GLuint shaderProgram;
		const GLuint modelLocation;
		const GLuint modelColorLocation;
	};
}

#endif