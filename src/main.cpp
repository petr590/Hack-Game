#include "models.h"
#include "block.h"
#include "player.h"
#include "enemy.h"
#include "shaders.h"
#include "globals.h"
#include "util.h"

#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <thread>

namespace hack_game {

	using std::cerr;
	using std::endl;
	using std::vector;
	using std::shared_ptr;

	using glm::uvec2;
	using glm::vec3;
	using glm::mat4;
	using glm::value_ptr;


	const GLuint BACKGROUND_COLOR = 0x636155;
	
	GLuint width = 0;
	GLuint height = 0;
	bool won = false;
	bool lost = false;
	
	static shared_ptr<Player> player = nullptr;
	static bool paused = false;


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
		 // Suppress compiler's warning
		(void)scancode;
		(void)mode;

		player->onKey(key, action);

		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);
		
		if (key == GLFW_KEY_F1 && action == GLFW_PRESS)
			paused = !paused;
	}


	static void empty_deleter(Entity*) {}
}

int main() {
	using namespace hack_game;

	GLFWwindow* window = initGL();
	// GLuint mainShaderProgram = createShaderProgram("shaders/main.vert", "shaders/main.frag", "shaders/main.geom");
	GLuint mainShaderProgram = createShaderProgram("shaders/main.vert", "shaders/main.frag");
	GLuint lightShaderProgram = createShaderProgram("shaders/light.vert", "shaders/light.frag");

	for (Model* model : Model::getModels()) {
		model->generateVertexArray();
	}
	
	// Настраиваем опции OpenGL
	glEnable(GL_DEPTH_TEST);
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	initShaderUniforms(mainShaderProgram, lightShaderProgram);

	const GLuint modelLoc           = glGetUniformLocation(mainShaderProgram, "model");
	const GLuint viewLoc            = glGetUniformLocation(mainShaderProgram, "view");
	const GLuint modelColorLoc      = glGetUniformLocation(mainShaderProgram, "modelColor");

	const GLuint lightModelLoc      = glGetUniformLocation(lightShaderProgram, "model");
	const GLuint lightViewLoc       = glGetUniformLocation(lightShaderProgram, "view");
	const GLuint lightModelColorLoc = glGetUniformLocation(lightShaderProgram, "modelColor");


	DrawContext mainDrawContext {
		.shaderProgram = mainShaderProgram,
		.modelLocation = modelLoc,
		.modelColorLocation = modelColorLoc,
	};

	DrawContext lightDrawContext {
		.shaderProgram = lightShaderProgram,
		.modelLocation = lightModelLoc,
		.modelColorLocation = lightModelColorLoc,
	};

	Block blocks[] = {
		Block::breakable(mainDrawContext, uvec2(2, 0)),
		Block::breakable(mainDrawContext, uvec2(3, 0)),
		Block::breakable(mainDrawContext, uvec2(4, 0)),

		Block::unbreakable(mainDrawContext, uvec2(5,  5)),
		Block::unbreakable(mainDrawContext, uvec2(10, 5)),
		Block::unbreakable(mainDrawContext, uvec2(10, 6)),
		Block::unbreakable(mainDrawContext, uvec2(11, 6)),
		Block::unbreakable(mainDrawContext, uvec2(10, 10)),
		Block::unbreakable(mainDrawContext, uvec2(11, 10)),
		Block::unbreakable(mainDrawContext, uvec2(10, 11)),
		Block::unbreakable(mainDrawContext, uvec2(11, 11)),
	};


	const size_t mapWidth = 20;
	const size_t mapHeight = 20;

	Map map(mapWidth, mapHeight);
	
	for (Block& block : blocks) {
		map[block.pos] = &block;
	}


	player = std::make_shared<Player>(
		mainDrawContext,
		lightDrawContext,
		0.25f,
		Camera(
			vec3(0.0f, 0.75f, 0.35f),
			// vec3(0.0f, 1.5f, 0.0f), // вид сверху
			vec3(0.0f, 0.0f, -0.05f)
		)
	);

	std::shared_ptr<Enemy> enemy = std::make_shared<Enemy1>(
		mainDrawContext, lightDrawContext,
		vec3(11.5f * TILE_SIZE, 0.0f, 5.5f * TILE_SIZE)
	);

	TickContext tickContext(map, player, enemy);

	for (Block& block : blocks) {
		tickContext.addEntity(shared_ptr<Entity>(&block, empty_deleter));
	}

	tickContext.updateEntities();

	const mat4 modelNoTransform(1.0f);
	const mat4 lightModel = glm::translate(modelNoTransform, lightPos);

	// Main loop
	for (float lastFrame = 0; !glfwWindowShouldClose(window);) {
		float currentFrame = glfwGetTime();
		tickContext.deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwPollEvents();

		// tick
		for (const auto& entity : tickContext.getEntities()) {
			entity->tick(tickContext);
		}

		tickContext.updateEntities();


		// shader uniforms
		glUseProgram(mainShaderProgram);
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(player->getCamera().getView()));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(modelNoTransform));

		glUseProgram(lightShaderProgram);
		glUniformMatrix4fv(lightModelLoc, 1, GL_FALSE, value_ptr(lightModel));
		glUniformMatrix4fv(lightViewLoc, 1, GL_FALSE, value_ptr(player->getCamera().getView()));
		

		// draw background
		constexpr vec3 background = colorAsVec3(BACKGROUND_COLOR);
		glClearColor(background.r, background.g, background.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// draw entities
		models::platform.draw(mainDrawContext);
		models::lightCube.draw(lightDrawContext);

		for (const auto& entity : tickContext.getEntities()) {
			entity->draw();
		}


		// swap buffers
		glfwSwapBuffers(window);


		// check paused
		if (paused) {
			while (paused && !glfwWindowShouldClose(window)) {
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
				glfwPollEvents();
			}

			lastFrame = glfwGetTime();
		}
	}
	
	glfwTerminate();
	return 0;
}