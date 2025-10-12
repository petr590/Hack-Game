#include "debug.h"

#include <cstdio>

#define GLEW_STATIC
#include <GL/glew.h>

namespace hack_game {

	void __glDebug(const char* file, int line) {
		GLenum error = glGetError();
		if (error != 0) {
			const char* name;

			switch (error) {
				case GL_INVALID_ENUM:      name = "GL_INVALID_ENUM";      break;
				case GL_INVALID_VALUE:     name = "GL_INVALID_VALUE";     break;
				case GL_INVALID_OPERATION: name = "GL_INVALID_OPERATION"; break;
				case GL_STACK_OVERFLOW:    name = "GL_STACK_OVERFLOW";    break;
				case GL_STACK_UNDERFLOW:   name = "GL_STACK_UNDERFLOW";   break;
				case GL_OUT_OF_MEMORY:     name = "GL_OUT_OF_MEMORY";     break;
				default:                   name = "unknown error";        break;
			}

			fprintf(stderr, "%s:%d: glGetError() = 0x%04x (%s)\n", file, line, error, name);
		}
	}
}