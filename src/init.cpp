#include "init.h"
#include "context/draw_context.h"
#include "model/models.h"
#include "entity/simple_entity.h"
#include "entity/player.h"
#include "entity/enemy.h"
#include "entity/block.h"
#include "entity/minion.h"

#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace hack_game {
	using std::cerr;
	using std::endl;
	using std::vector;
	using std::shared_ptr;
	using std::make_shared;

	using glm::vec3;
	using glm::uvec2;

	int windowWidth = 0;
	int windowHeight = 0;
	int refreshRate = 0;


	GLFWwindow* initGLFW() {
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_SAMPLES, 4);
		glfwWindowHint(GLFW_ALPHA_BITS, 8);

		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);

		windowWidth = mode->width;
		windowHeight = mode->height;
		refreshRate = mode->refreshRate;
		
		GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "LearnOpenGL", nullptr, nullptr);
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

	void initGL() {
		for (Model* model : Model::getModels()) {
			model->generateVertexArray();
		}
		
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glEnable(GL_MULTISAMPLE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}


	static void empty_deleter(Entity*) {}

	static vector<Block> blocks;


	TickContext createTickContext(DrawContext& drawContext) {
		Shader& mainShader = drawContext.mainShader;

		blocks.reserve(12);

		blocks.push_back(Block::breakable(mainShader, uvec2(2, 0)));
		blocks.push_back(Block::breakable(mainShader, uvec2(3, 0)));
		blocks.push_back(Block::breakable(mainShader, uvec2(4, 0)));

		blocks.push_back(Block::unbreakable(mainShader, uvec2( 3,  9)));
		blocks.push_back(Block::unbreakable(mainShader, uvec2( 3, 10)));
		blocks.push_back(Block::unbreakable(mainShader, uvec2( 3, 11)));
		blocks.push_back(Block::unbreakable(mainShader, uvec2(16,  9)));
		blocks.push_back(Block::unbreakable(mainShader, uvec2(16, 10)));
		blocks.push_back(Block::unbreakable(mainShader, uvec2(16, 11)));
		blocks.push_back(Block::unbreakable(mainShader, uvec2( 9, 17)));
		blocks.push_back(Block::unbreakable(mainShader, uvec2(10, 17)));
		blocks.push_back(Block::unbreakable(mainShader, uvec2(11, 17)));


		const size_t mapWidth = 20;
		const size_t mapHeight = 20;

		Map map(mapWidth, mapHeight);
		
		for (Block& block : blocks) {
			map[block.pos] = &block;
		}


		player = make_shared<Player>(
			drawContext,
			0.25f,
			Camera(
				vec3(0.0f, 0.75f, 0.35f),
				// vec3(0.0f, 1.5f, 0.0f), // вид сверху
				vec3(0.0f, 0.0f, -0.05f) // вид сверху и сбоку
			)
		);

		shared_ptr<Enemy> enemy = make_shared<Enemy1>(
			drawContext,
			vec3(10.5f * TILE_SIZE, 0.0f, 10.5f * TILE_SIZE)
		);

		TickContext tickContext(std::move(map), player, enemy);

		for (Block& block : blocks) {
			tickContext.addEntity(shared_ptr<Entity>(&block, empty_deleter));
		}

		tickContext.addEntity(make_shared<SimpleEntity>(drawContext.mainShader, models::platform));
		tickContext.addEntity(make_shared<Minion>(drawContext, vec3(12.5f * TILE_SIZE, 0.0f, 10.5f * TILE_SIZE)));
		// tickContext.addEntity(make_shared<SimpleEntity>(drawContext.lightShader, models::lightCube));

		tickContext.updateEntities();

		return tickContext;
	}
}