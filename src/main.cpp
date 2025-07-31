#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "block.h"
#include "models.h"
#include "camera.h"

namespace hack_game {
	const GLuint WIDTH = 800, HEIGHT = 600;
	const GLuint LOG_SIZE = 512;

	const GLuint BACKGROUND_COLOR = 0x3A3A2B;


	using std::vector;
	using std::cerr;
	using std::endl;

	using glm::vec3;
	using glm::mat4;
	using glm::translate;
	using glm::rotate;
	using glm::scale;
	using glm::radians;
	using glm::perspective;
	using glm::value_ptr;


	static constexpr vec3 colorAsVec3(GLuint color) {
		return vec3(
			((color >> 16) & 0xFF) / 255.0f,
			((color >>  8) & 0xFF) / 255.0f,
			((color      ) & 0xFF) / 255.0f
		);
	}

	static GLuint compileShader(GLenum type, const char* filename) {
		std::ifstream file(filename, std::ios::ate);
		const size_t fsize = file.tellg();
		
		auto source = std::make_unique<char[]>(fsize + 1);
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
			glGetShaderInfoLog(shader, LOG_SIZE, NULL, infoLog);
			
			const char* strType =
					type == GL_VERTEX_SHADER ? "VERTEX" :
					type == GL_FRAGMENT_SHADER ? "FRAGMENT" : "UNKNOWN";
			
			cerr << "ERROR::SHADER::" << strType << "::COMPILATION_FAILED\n" << infoLog << endl;
			exit(0);
		}
		
		return shader;
	}


	static GLuint createShaderProgram(const char* vertexShaderPath, const char* fragmentShaderPath, const char* geometryShaderPath = nullptr) {
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


	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);

	static GLFWwindow* initGL() {
		// Инициализируем GLFW
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		// glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
		
		GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
		if (window == nullptr) {
			cerr << "Failed to create GLFW window" << endl;
			glfwTerminate();
			exit(1);
		}
		
		glfwMakeContextCurrent(window);
		glfwSetKeyCallback(window, keyCallback);
		
		// Инициализируем GLEW
		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK) {
			cerr << "Failed to initialize GLEW" << endl;
			glfwTerminate();
			exit(1);
		}
		
		
		// Устанавливаем viewport
		GLint width, height;
		glfwGetFramebufferSize(window, &width, &height);  
		glViewport(0, 0, width, height);

		return window;
	}

	
	static Camera camera(
		0.5f,
		vec3(0.0f, 0.8f, 0.4f),
		vec3(0.0f, 0.0f, 0.0f)
	);

	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
		camera.onKey(key, action);

		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);
	}
}


int main() {
	using namespace hack_game;

	GLFWwindow* window = initGL();
	// GLuint mainShaderProgram = createShaderProgram("shaders/main.vert", "shaders/main.frag", "shaders/main.geom");
	GLuint mainShaderProgram = createShaderProgram("shaders/main.vert", "shaders/main.frag");
	GLuint lightShaderProgram = createShaderProgram("shaders/light.vert", "shaders/light.frag");

	platform.generateVertexArray();
	cube.generateVertexArray();
	mc.generateVertexArray();
	
	// Настраиваем опции OpenGL
	glEnable(GL_DEPTH_TEST);
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	GLuint modelLoc       = glGetUniformLocation(mainShaderProgram, "model");
	GLuint viewLoc        = glGetUniformLocation(mainShaderProgram, "view");
	GLuint projectionLoc  = glGetUniformLocation(mainShaderProgram, "projection");
	GLuint lightColorLoc  = glGetUniformLocation(mainShaderProgram, "lightColor");
	GLuint lightPosLoc    = glGetUniformLocation(mainShaderProgram, "lightPos");
	GLuint vertexColorLoc = glGetUniformLocation(mainShaderProgram, "vertexColor");

	GLuint lightModelLoc       = glGetUniformLocation(lightShaderProgram, "model");
	GLuint lightViewLoc        = glGetUniformLocation(lightShaderProgram, "view");
	GLuint lightProjectionLoc  = glGetUniformLocation(lightShaderProgram, "projection");
	GLuint lightLightColorLoc  = glGetUniformLocation(lightShaderProgram, "lightColor");

	glUseProgram(mainShaderProgram);

	mat4 projection = perspective(45.0f, GLfloat(WIDTH) / GLfloat(HEIGHT), 0.1f, 100.0f);
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, value_ptr(projection));

	vec3 lightColor(1.0f, 1.0f, 1.0f);
	glUniform3fv(lightColorLoc, 1, value_ptr(lightColor));
	

	vec3 lightPos(1.0f, 0.75f, -1.0f);
	glUniform3fv(lightPosLoc, 1, value_ptr(lightPos));


	glUseProgram(lightShaderProgram);
	glUniformMatrix4fv(lightProjectionLoc, 1, GL_FALSE, value_ptr(projection));
	glUniform3fv(lightLightColorLoc, 1, value_ptr(lightColor));

	mat4 lightModel(1.0f);
	lightModel = translate(lightModel, lightPos);
	glUniformMatrix4fv(lightModelLoc, 1, GL_FALSE, value_ptr(lightModel));
	

	Block blocks[] = {
		Block(true, glm::ivec2(-5, 0), cube, mainShaderProgram, modelLoc),
		Block(true, glm::ivec2(-4, 0), cube, mainShaderProgram, modelLoc),
		// Block(true, glm::ivec2()),
	};


	GLfloat lastFrame = 0;
	
	// Main loop
	while (!glfwWindowShouldClose(window)) {
		GLfloat currentFrame = glfwGetTime();
		GLfloat deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwPollEvents();
		
		vec3 background = colorAsVec3(BACKGROUND_COLOR);
		glClearColor(background.r, background.g, background.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		camera.move(deltaTime);

		glUseProgram(mainShaderProgram);
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(camera.getView()));

		mat4 model(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));
		platform.draw(vertexColorLoc);

		mat4 mcModel(1.0f);
		mcModel = scale(mcModel, vec3(8.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(mcModel));
		mc.draw(vertexColorLoc);
		
		for (const Block& block : blocks) {
			block.draw(vertexColorLoc);
		}


		glUseProgram(lightShaderProgram);
		glUniformMatrix4fv(lightViewLoc, 1, GL_FALSE, value_ptr(camera.getView()));

		cube.draw(0);

		glfwSwapBuffers(window);
	}
	
	glfwTerminate();
	return 0;
}