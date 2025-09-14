#ifndef HACK_GAME__CONTEXT__SHADER_H
#define HACK_GAME__CONTEXT__SHADER_H

#include "gl_fwd.h"
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
		const GLint texture0;
		const GLint texture1;
	
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
		void setTexture0(GLint);
		void setTexture1(GLint);
	};
}

#endif