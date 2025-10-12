#include "draw_context.h"
#include <glm/gtc/matrix_transform.hpp>

namespace hack_game {

	using glm::vec3;
	using glm::mat4;

	static constexpr vec3 lightColor(1.0f, 1.0f, 1.0f);
	static constexpr vec3 lightPos(0.5f, 0.6f, 0.5f);


	void DrawContext::initShaders(int windowWidth, int windowHeight) {
		const mat4 projection = glm::perspective(45.0f, float(windowWidth) / float(windowHeight), 0.1f, 100.0f);

		mainShader.use();
		mainShader.setUniform("projection", projection);
		mainShader.setUniform("lightColor", lightColor);
		mainShader.setUniform("lightPos",   lightPos);

		for (auto& entry : shaders) {
			entry.second.use();
			entry.second.setUniform("projection", projection);
		}
	}


	Shader& DrawContext::getShader(const char* name) {
		return shaders.at(name);
	}

	void DrawContext::addShader(Shader&& shader) {
		shaders.emplace(shader.getName(), std::move(shader));
	}
}