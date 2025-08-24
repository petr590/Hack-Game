#include "shaders.h"

#include <iostream>
#include <fstream>
#include <memory>

#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace hack_game {
	using std::cerr;
	using std::endl;
	using std::ios;
	using std::ifstream;
	using std::streamsize;
	using std::make_unique;

	using glm::mat4;
	using glm::perspective;
	using glm::value_ptr;

	const size_t LOG_SIZE = 512;

	GLuint compileShader(GLenum type, const char* filename) {
		ifstream file(filename, ios::ate);
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
		GLint success;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		
		if (!success) {
			GLchar infoLog[LOG_SIZE];
			glGetShaderInfoLog(shader, LOG_SIZE, nullptr, infoLog);
			
			const char* strType =
					type == GL_VERTEX_SHADER ? "VERTEX" :
					type == GL_FRAGMENT_SHADER ? "FRAGMENT" : "UNKNOWN";
			
			cerr << "ERROR::SHADER::" << strType << "::COMPILATION_FAILED\n" << infoLog << endl;
			exit(0);
		}
		
		return shader;
	}


	GLuint createShaderProgram(const char* vertexShaderPath, const char* fragmentShaderPath, const char* geometryShaderPath) {
		// Читаем и компилируем шейдеры
		GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderPath);
		GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderPath);

		GLuint geometryShader = 0;
		if (geometryShaderPath != nullptr) {
			geometryShader = compileShader(GL_GEOMETRY_SHADER, geometryShaderPath);
		}
		
		if (vertexShader == 0 || fragmentShader == 0 || (geometryShader == 0 && geometryShaderPath != nullptr)) {
			glfwTerminate();
			exit(1);
		}
		
		
		// Связываем шейдеры в программу
		GLuint shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		if (geometryShader) glAttachShader(shaderProgram, geometryShader);

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
		if (geometryShader) glDeleteShader(geometryShader);

		return shaderProgram;
	}

	void initShaderUniforms(GLuint mainShader, std::initializer_list<GLuint> otherShaders) {
		const mat4 projection = perspective(45.0f, GLfloat(width) / GLfloat(height), 0.1f, 100.0f);
		
		glUseProgram(mainShader);

		GLint projectionUniform = glGetUniformLocation(mainShader, "projection");
		GLint lightColorUniform = glGetUniformLocation(mainShader, "lightColor");
		GLint lightPosUniform   = glGetUniformLocation(mainShader, "lightPos");

		glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, value_ptr(projection));
		glUniform3fv(lightColorUniform, 1, value_ptr(lightColor));
		glUniform3fv(lightPosUniform, 1, value_ptr(lightPos));


		for (GLuint program : otherShaders) {
			glUseProgram(program);

			projectionUniform = glGetUniformLocation(program, "projection");
			glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, value_ptr(projection));
		}
	}
}
