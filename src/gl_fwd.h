#ifndef HACK_GAME__GL_FWD_H
#define HACK_GAME__GL_FWD_H

#include <cstdint>

namespace hack_game {
	using GLint = int32_t; // Полностью совместим с GLint из <GL/glew.h>
	using GLuint = uint32_t; // Полностью совместим с GLuint из <GL/glew.h>
	using GLenum = uint32_t; // Полностью совместим с GLenum из <GL/glew.h>
}

#endif
