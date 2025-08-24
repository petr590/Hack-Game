#include "shaders.h"
#include "entity/player.h"
#include "entity/enemy.h"
#include "entity/block.h"
#include "entity/animation_entity.h"
#include "context/tick_context.h"
#include "context/draw_context.h"
#include "model/models.h"
#include "util.h"

#include <iostream>
#include <thread>
#include <chrono>

#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

namespace hack_game {

	using std::cerr;
	using std::endl;
	using std::map;
	using std::vector;
	using std::shared_ptr;
	using std::make_shared;

	using glm::uvec2;
	using glm::vec3;
	using glm::mat4;
	using glm::value_ptr;


	const GLuint BACKGROUND_COLOR = 0x636155;
	
	int width = 0;
	int height = 0;
	bool won = false;
	bool lost = false;
	
	static shared_ptr<Player> player = nullptr;
	static bool paused = false;


	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);

	static GLFWwindow* initGLFW() {
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		// glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
		glfwWindowHint(GLFW_SAMPLES, 4);

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


	static DrawContext createDrawContext(GLuint shader) {
		return {
			.shaderProgram     = shader,
			.modelUniform      = glGetUniformLocation(shader, "model"),
			.viewUniform       = glGetUniformLocation(shader, "view"),
			.modelColorUniform = glGetUniformLocation(shader, "modelColor"),
		};
	}


	static void empty_deleter(Entity*) {}

	static vector<Block> blocks;


	static TickContext createTickContext(
			DrawContext& mainDrawContext,
			DrawContext& lightDrawContext,
			DrawContext& animationDrawContext
	) {
		blocks.reserve(11);

		blocks.push_back(Block::breakable(mainDrawContext, uvec2(2, 0)));
		blocks.push_back(Block::breakable(mainDrawContext, uvec2(3, 0)));
		blocks.push_back(Block::breakable(mainDrawContext, uvec2(4, 0)));

		blocks.push_back(Block::unbreakable(mainDrawContext, uvec2(5,  5)));
		blocks.push_back(Block::unbreakable(mainDrawContext, uvec2(10, 5)));
		blocks.push_back(Block::unbreakable(mainDrawContext, uvec2(10, 6)));
		blocks.push_back(Block::unbreakable(mainDrawContext, uvec2(11, 6)));
		blocks.push_back(Block::unbreakable(mainDrawContext, uvec2(10, 10)));
		blocks.push_back(Block::unbreakable(mainDrawContext, uvec2(11, 10)));
		blocks.push_back(Block::unbreakable(mainDrawContext, uvec2(10, 11)));
		blocks.push_back(Block::unbreakable(mainDrawContext, uvec2(11, 11)));


		const size_t mapWidth = 20;
		const size_t mapHeight = 20;

		Map map(mapWidth, mapHeight);
		
		for (Block& block : blocks) {
			map[block.pos] = &block;
		}


		player = make_shared<Player>(
			mainDrawContext,
			lightDrawContext,
			0.25f,
			Camera(
				vec3(0.0f, 0.75f, 0.35f),
				// vec3(0.0f, 1.5f, 0.0f), // вид сверху
				vec3(0.0f, 0.0f, -0.05f) // вид сверху и сбоку
			)
		);

		shared_ptr<Enemy> enemy = make_shared<Enemy1>(
			mainDrawContext, lightDrawContext,
			vec3(11.5f * TILE_SIZE, 0.0f, 5.5f * TILE_SIZE)
		);

		TickContext tickContext(std::move(map), player, enemy);

		for (Block& block : blocks) {
			tickContext.addEntity(shared_ptr<Entity>(&block, empty_deleter));
		}

		tickContext.addEntity(make_shared<SimpleEntity>(mainDrawContext, models::platform));
		tickContext.addEntity(make_shared<SimpleEntity>(lightDrawContext, models::lightCube));
		tickContext.addEntity(make_shared<AnimationEntity>(animationDrawContext));

		tickContext.updateEntities();

		return tickContext;
	}


	static void tick(TickContext& tickContext) {

		for (auto& entry : tickContext.getEntityMap()) {
			for (const auto& entity : entry.second) {
				entity->tick(tickContext);
			}
		}

		tickContext.updateEntities();
	}


	static void render(GLFWwindow* window, TickContext& tickContext, const map<GLuint, DrawContext*>& drawContextByShader) {

		for (auto& entry : tickContext.getEntityMap()) {
			GLuint shader = entry.first;

			glUseProgram(shader);
			glUniformMatrix4fv(
				drawContextByShader.at(shader)->viewUniform,
				1, GL_FALSE,
				value_ptr(player->getCamera().getView())
			);
			

			for (const auto& entity : entry.second) {
				entity->draw();
			}
		}

		glfwSwapBuffers(window);
	}


	static void loop(GLFWwindow* window, TickContext& tickContext, const map<GLuint, DrawContext*>& drawContextByShader) {

		float deltaTimeSum = 0;
		float drawTimeSum = 0;
		int drawTimes = 0;

		for (float lastFrame = 0; !glfwWindowShouldClose(window);) {
			// update time
			float currentFrame = glfwGetTime();
			tickContext.deltaTime = currentFrame - lastFrame;
			deltaTimeSum += tickContext.deltaTime;
			lastFrame = currentFrame;

			// tick
			glfwPollEvents();
			tick(tickContext);

			// draw background
			constexpr vec3 background = colorAsVec3(BACKGROUND_COLOR);
			glClearColor(background.r, background.g, background.b, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			
			// render
			float drawStartTime = glfwGetTime();
			render(window, tickContext, drawContextByShader);
			float drawEndTime = glfwGetTime();

			// print fps
			drawTimeSum += drawEndTime - drawStartTime;
			drawTimes++;
			
			if (deltaTimeSum >= 1) {
				printf("  %.2f ms, %d fps\t\t\r",
						1000.0f * (drawTimeSum / drawTimes),
						int(drawTimes / drawTimeSum)
				);
				fflush(stdout);

				drawTimes = 0;
				drawTimeSum = 0;
				deltaTimeSum -= 1;
			}


			// check paused
			if (paused) {
				while (paused && !glfwWindowShouldClose(window)) {
					std::this_thread::sleep_for(std::chrono::milliseconds(100));
					glfwPollEvents();
				}

				lastFrame = glfwGetTime();
			}
		}
		
		printf("\n");
	}
}

int main() {
	using namespace hack_game;

	GLFWwindow* window = initGLFW();

	for (Model* model : Model::getModels()) {
		model->generateVertexArray();
	}
	
	// Настраиваем опции OpenGL
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	

	GLuint mainShader      = createShaderProgram("shaders/main.vert",      "shaders/main.frag");
	GLuint lightShader     = createShaderProgram("shaders/light.vert",     "shaders/light.frag");
	GLuint animationShader = createShaderProgram("shaders/animation.vert", "shaders/animation.frag");

	initShaderUniforms(mainShader, { lightShader, animationShader });

	DrawContext mainDrawContext = createDrawContext(mainShader);
	DrawContext lightDrawContext = createDrawContext(lightShader);
	DrawContext animationDrawContext = createDrawContext(animationShader);

	const map<GLuint, DrawContext*> drawContextByShader {
		{ mainShader,      &mainDrawContext      },
		{ lightShader,     &lightDrawContext     },
		{ animationShader, &animationDrawContext },
	};


	TickContext tickContext	= createTickContext(mainDrawContext, lightDrawContext, animationDrawContext);
	loop(window, tickContext, drawContextByShader);

	glfwTerminate();
	return 0;
}
