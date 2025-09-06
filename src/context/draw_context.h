#ifndef HACK_GAME_DRAW_CONTEXT_H
#define HACK_GAME_DRAW_CONTEXT_H

#include "gl_fwd.h"
#include <map>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace hack_game {

	class Shader {
	public:
		const GLuint id;

	private:
		const GLint model;
		const GLint view;
		const GLint modelColor;
		const GLint centerPos;
		const GLint progress;
		const GLint mode;
		const GLint seed;
	
	public:
		Shader() noexcept;
		explicit Shader(GLuint shaderProgram);

		void setModel(const glm::mat4&);
		void setView(const glm::mat4&);
		void setModelColor(const glm::vec3&);
		
		void setCenterPos(const glm::vec3&);
		void setProgress(float);
		void setMode(GLuint);
		void setSeed(GLint);
	};


	struct DrawContext {
		Shader nullShader; // Пустой шейдер, id = 0
		Shader mainShader;
		Shader lightShader;

		std::map<std::string, Shader> shaders;
	};
}

#endif