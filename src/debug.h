#ifndef HACK_GAME__DEBUG_H
#define HACK_GAME__DEBUG_H

#ifndef NDEBUG

#define GL_DEBUG() hack_game::__glDebug(__FILE__, __LINE__)

namespace hack_game {
	void __glDebug(const char* file, int line);
}

#else
#define GL_DEBUG()
#endif // NDEBUG

#endif