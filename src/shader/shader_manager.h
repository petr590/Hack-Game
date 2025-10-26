#ifndef HACK_GAME__SHADER__SHADER_MANAGER_H
#define HACK_GAME__SHADER__SHADER_MANAGER_H

#include "shader.h"
#include <map>
#include <string>

namespace hack_game {

	struct ShaderManager {
		Shader nullShader; // Пустой шейдер, id = 0
		Shader mainShader;

	private:
		std::map<std::string_view, Shader> shaders;
		std::map<GLuint, Shader*> shadersById;

	public:
		template<typename... Shaders>
		ShaderManager(int windowWidth, int windowHeight, Shader&& nullShader, Shader&& mainShader, Shaders&&... shaders):
				nullShader(std::move(nullShader)),
				mainShader(std::move(mainShader)) {
			
			(addShader(std::forward<Shaders>(shaders)), ...);
			initShaders(windowWidth, windowHeight);
		}

		std::map<std::string_view, Shader>& getShaders() noexcept {
			return shaders;
		}

		const std::map<GLuint, Shader*>& getShadersById() const noexcept {
			return shadersById;
		}

		Shader& getShader(const char* name);
		Shader& getShader(GLuint id);

		void updateWindowSize(GLint width, GLint height);
	
	private:
		void addShader(Shader&&);
		void initShaders(int windowWidth, int windowHeight);
	};
}

#endif