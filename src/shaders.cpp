#include "shaders.h"
#include "context/draw_context.h"
#include "dir_paths.h"

#include <iostream>
#include <fstream>
#include <string>
#include <memory>

#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace hack_game {
	using std::cout;
	using std::cerr;
	using std::endl;
	using std::string;
	using std::string_view;
	using std::ios;
	using std::ifstream;
	using std::streamsize;
	using std::make_unique;

	using glm::mat4;
	using glm::perspective;
	using glm::value_ptr;

	const size_t LOG_SIZE = 512;

	GLuint compileShader(GLenum type, const char* path) {
		ifstream file(path, ios::ate);

		if (file.fail()) {
			cerr << "Cannot open file \"" << path << "\"" << endl;
			exit(1);
		}

		const streamsize fsize = file.tellg();
		
		auto source = make_unique<char[]>(size_t(fsize + 1));
		char* const ptr = source.get();
		ptr[fsize] = '\0';
		
		file.seekg(0);
		file.read(ptr, fsize);
		file.close();
		
		GLuint shader = glCreateShader(type);
		glShaderSource(shader, 1, &ptr, nullptr);
		
		source.reset();
		
		
		glCompileShader(shader);

		GLint logLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

		string log(size_t(logLength), '\0');
		glGetShaderInfoLog(shader, logLength, nullptr, log.data());

		GLint success;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

		if (success) {
			if (logLength > 0) {
				cout << log << endl;
			}

		} else {
			const char* strType =
					type == GL_VERTEX_SHADER ? "VERTEX" :
					type == GL_FRAGMENT_SHADER ? "FRAGMENT" : "UNKNOWN";
			
			cerr << "ERROR::SHADER::" << strType << "::COMPILATION_FAILED\n" << log << endl;
			exit(1);
		}
		
		return shader;
	}


	static GLuint createShaderProgram0(const char* vertexShaderPath, const char* fragmentShaderPath) {
		// Читаем и компилируем шейдеры
		GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderPath);
		GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderPath);

		if (vertexShader == 0 || fragmentShader == 0) {
			glfwTerminate();
			exit(1);
		}
		
		
		// Связываем шейдеры в программу
		GLuint shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);

		glLinkProgram(shaderProgram);
		
		GLint success;
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success) {
			GLchar infoLog[LOG_SIZE];
			glGetProgramInfoLog(shaderProgram, LOG_SIZE, NULL, infoLog);
			cerr << "ERROR::PROGRAM::LINKAGE_FAILED\n" << infoLog << endl;
			return 0;
		}
		
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		return shaderProgram;
	}

	GLuint createShaderProgram(const char* vertexShaderName, const char* fragmentShaderName) {
		string vertexShaderPath   = string(SHADERS_DIR) + vertexShaderName;
		string fragmentShaderPath = string(SHADERS_DIR) + fragmentShaderName;
		return createShaderProgram0(vertexShaderPath.c_str(), fragmentShaderPath.c_str());
	}

	GLuint createAnimationShaderProgram(const char* vertexShaderName, const char* fragmentShaderName) {
		string vertexShaderPath   = string(SHADERS_ANIMATION_DIR) + vertexShaderName;
		string fragmentShaderPath = string(SHADERS_ANIMATION_DIR) + fragmentShaderName;
		return createShaderProgram0(vertexShaderPath.c_str(), fragmentShaderPath.c_str());
	}


	static void setProjection(GLuint shaderId, const mat4& projection) {
		glUseProgram(shaderId);

		GLint projectionUniform = glGetUniformLocation(shaderId, "projection");
		glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, value_ptr(projection));
	}


	void initShaderUniforms(DrawContext& drawContext) {
		const mat4 projection = perspective(45.0f, GLfloat(windowWidth) / GLfloat(windowHeight), 0.1f, 100.0f);
		const GLuint mainShaderId = drawContext.mainShader.id;

		glUseProgram(mainShaderId);

		const GLint lightColorUniform = glGetUniformLocation(mainShaderId, "lightColor");
		const GLint lightPosUniform   = glGetUniformLocation(mainShaderId, "lightPos");

		glUniform3fv(lightColorUniform, 1, value_ptr(lightColor));
		glUniform3fv(lightPosUniform, 1, value_ptr(lightPos));

		setProjection(drawContext.mainShader.id, projection);
		setProjection(drawContext.lightShader.id, projection);

		for (const auto& entry : drawContext.shaders) {
			setProjection(entry.second.id, projection);
		}
	}
}
