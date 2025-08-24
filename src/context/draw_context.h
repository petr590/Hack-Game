#ifndef HACK_GAME_DRAW_CONTEXT_H
#define HACK_GAME_DRAW_CONTEXT_H

#include "../gl_fwd.h"

namespace hack_game {

	struct DrawContext {
		const GLuint shaderProgram;

		const GLint modelUniform;
		const GLint viewUniform;
		const GLint modelColorUniform;
	};
}

#endif
