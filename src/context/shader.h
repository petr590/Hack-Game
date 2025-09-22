#ifndef HACK_GAME__CONTEXT__SHADER_H
#define HACK_GAME__CONTEXT__SHADER_H

#include "gl_fwd.h"
#include <array>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace hack_game {

	class Shader {
	public:
		using TextureArray = std::array<GLint, 4>;

	private:
		const GLuint id;
		const char* const name;

		const GLint model;
		const GLint view;
		const GLint modelColor;
		const GLint modelBrightness;
		const GLint centerPos;
		const GLint pixelSize;
		const GLint progress;
		const GLint mode;
		const GLint seed;
		const TextureArray textures;
	
	public:
		explicit Shader(const char* name) noexcept;
		Shader(const char* name, GLuint shaderProgram);

		GLuint getId() const noexcept {
			return id;
		}

		void setModel(const glm::mat4&);
		void setView(const glm::mat4&);
		void setModelColor(const glm::vec3&);
		void setModelBrightness(float);
		
		void setCenterPos(const glm::vec3&);
		void setPixelSize(const glm::vec2&);
		void setProgress(float);
		void setMode(GLuint);
		void setSeed(GLint);
	};
}

#endif