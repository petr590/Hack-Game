#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "block.h"
#include "models.h"
#include "player.h"
#include "shaders.h"

namespace hack_game {
	GLuint width = 0;
	GLuint height = 0;

	const GLuint BACKGROUND_COLOR = 0x3A3A2B;


	using std::cerr;
	using std::endl;
	using std::vector;
	using boost::dynamic_bitset;

	using glm::vec3;
	using glm::mat4;
	using glm::value_ptr;

	
	static Player player(
		mc,
		0.5f,
		Camera(
			vec3(0.0f, 0.8f, 0.4f),
			vec3(0.0f, 0.0f, 0.0f)
		)
	);


	static constexpr vec3 colorAsVec3(GLuint color) {
		return vec3(
			((color >> 16) & 0xFF) / 255.0f,
			((color >>  8) & 0xFF) / 255.0f,
			((color      ) & 0xFF) / 255.0f
		);
	}


	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);

	static GLFWwindow* initGL() {
		// Инициализируем GLFW
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		// glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);

		width = mode->width;
		height = mode->height;
		
		GLFWwindow* window = glfwCreateWindow(width, height, "LearnOpenGL", nullptr, nullptr);
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

	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
		player.onKey(key, action);

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

	initShaderUniforms(mainShaderProgram, lightShaderProgram);

	GLuint modelLoc      = glGetUniformLocation(mainShaderProgram, "model");
	GLuint viewLoc       = glGetUniformLocation(mainShaderProgram, "view");
	GLuint modelColorLoc = glGetUniformLocation(mainShaderProgram, "modelColor");
	GLuint lightViewLoc  = glGetUniformLocation(lightShaderProgram, "view");


	const size_t mapWidth = 20;
	const size_t mapHeight = 20;

	Block blocks[] = {
		Block(glm::ivec2(10, 10), cube, mainShaderProgram, modelLoc),
		Block(glm::ivec2(11, 10), cube, mainShaderProgram, modelLoc),
		Block(glm::ivec2(10, 11), cube, mainShaderProgram, modelLoc),
		Block(glm::ivec2(11, 11), cube, mainShaderProgram, modelLoc),
	};

	vector<dynamic_bitset<>> map(mapWidth, dynamic_bitset<>(mapHeight));
	for (Block& block : blocks) {
		map[block.pos.x][block.pos.y] = true;
	}

	for (size_t y = 0; y < mapHeight; y++) {
		for (size_t x = 0; x < mapWidth; x++) {
			printf("%d ", bool(map[x][y]));
		}

		printf("\n");
	}


	// Main loop
	for (float lastFrame = 0; !glfwWindowShouldClose(window);) {
		float currentFrame = glfwGetTime();
		float deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwPollEvents();
		
		constexpr vec3 background = colorAsVec3(BACKGROUND_COLOR);
		glClearColor(background.r, background.g, background.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		player.move(deltaTime, map);

		glUseProgram(mainShaderProgram);
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(player.getCamera().getView()));

		mat4 model(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));
		platform.draw(modelColorLoc);

		player.draw(modelLoc, modelColorLoc);
		
		for (const Block& block : blocks) {
			block.draw(modelColorLoc);
		}


		glUseProgram(lightShaderProgram);
		glUniformMatrix4fv(lightViewLoc, 1, GL_FALSE, value_ptr(player.getCamera().getView()));

		cube.draw(0);

		glfwSwapBuffers(window);
	}
	
	glfwTerminate();
	return 0;
}