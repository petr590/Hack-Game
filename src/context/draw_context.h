#ifndef HACK_GAME_DRAW_CONTEXT_H
#define HACK_GAME_DRAW_CONTEXT_H

#include "shader.h"
#include <map>
#include <string>

namespace hack_game {

	struct DrawContext {
		Shader nullShader; // Пустой шейдер, id = 0
		Shader mainShader;

	private:
		std::map<std::string_view, Shader> shaders;

	public:
		template<typename... Shaders>
		DrawContext(int windowWidth, int windowHeight, Shader&& nullShader, Shader&& mainShader, Shaders&&... shaders):
				nullShader(std::move(nullShader)),
				mainShader(std::move(mainShader)) {
			
			(addShader(std::forward<Shaders>(shaders)), ...);
			initShaders(windowWidth, windowHeight);
		}

		std::map<std::string_view, Shader>& getShaders() noexcept {
			return shaders;
		}

		Shader& getShader(const char* name);
	
	private:
		void addShader(Shader&&);
		void initShaders(int windowWidth, int windowHeight);
	};
}

#endif