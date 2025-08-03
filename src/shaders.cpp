#include "shaders.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <memory>

namespace hack_game {
	using std::cerr;
	using std::endl;
	using std::ios;
	using std::ifstream;
	using std::make_unique;

	using glm::vec3;
	using glm::mat4;
	using glm::perspective;
	using glm::value_ptr;

	const size_t LOG_SIZE = 512;

	GLuint compileShader(GLenum type, const char* filename) {
		ifstream file(filename, ios::ate);
		const size_t fsize = file.tellg();
		
		auto source = make_unique<char[]>(fsize + 1);
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
			return -1;
		}
		
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		if (geometryShader) glDeleteShader(geometryShader);

		return shaderProgram;
	}

	void initShaderUniforms(GLuint mainShaderProgram, GLuint lightShaderProgram) {
		const mat4 projection = perspective(45.0f, GLfloat(width) / GLfloat(height), 0.1f, 100.0f);
		
		
		glUseProgram(mainShaderProgram);

		GLuint projectionLoc = glGetUniformLocation(mainShaderProgram, "projection");
		GLuint lightColorLoc = glGetUniformLocation(mainShaderProgram, "lightColor");
		GLuint lightPosLoc   = glGetUniformLocation(mainShaderProgram, "lightPos");

		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, value_ptr(projection));
		glUniform3fv(lightColorLoc, 1, value_ptr(lightColor));
		glUniform3fv(lightPosLoc, 1, value_ptr(lightPos));


		glUseProgram(lightShaderProgram);

		GLuint lightProjectionLoc = glGetUniformLocation(lightShaderProgram, "projection");

		glUniformMatrix4fv(lightProjectionLoc, 1, GL_FALSE, value_ptr(projection));
	}
}