#ifndef HACK_GAME__ENTITY__ANIMATION__CUBE_PARTICLE_MODE_H
#define HACK_GAME__ENTITY__ANIMATION__CUBE_PARTICLE_MODE_H

#include "gl_fwd.h"

namespace hack_game {

	enum class Mode: GLuint {
		FADING = 0,
		SOLID  = 1,
	};
}

#endif