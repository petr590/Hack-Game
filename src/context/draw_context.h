#ifndef HACK_GAME_DRAW_CONTEXT_H
#define HACK_GAME_DRAW_CONTEXT_H

#include "shader.h"
#include <map>
#include <string>

namespace hack_game {

	struct DrawContext {
		Shader nullShader; // Пустой шейдер, id = 0
		Shader mainShader;
		Shader lightShader;

		std::map<std::string, Shader> shaders;
	};
}

#endif