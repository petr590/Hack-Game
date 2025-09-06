#include "init.h"
#include "shaders.h"
#include "entity/player.h"
#include "entity/enemy.h"
#include "entity/block.h"
#include "context/tick_context.h"
#include "context/draw_context.h"
#include "model/models.h"
#include "util.h"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <thread>
#include <chrono>

#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

namespace hack_game {

	using std::ofstream;
	using std::map;
	using std::shared_ptr;

	using glm::vec3;


	const GLuint BACKGROUND_COLOR = 0x636155;
	
	bool won = false;
	bool lost = false;
	shared_ptr<Player> player = nullptr;

	static bool paused = false;
	static bool nextFrame = false;


	void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
		// Suppress compiler's warnings
		(void)scancode;
		(void)mode;

		player->onKey(key, action);

		if (action == GLFW_PRESS) {
			switch (key) {
				case GLFW_KEY_ESCAPE: glfwSetWindowShouldClose(window, GL_TRUE); break;
				case GLFW_KEY_F1: paused = !paused; break;
				case GLFW_KEY_F2: if (paused) nextFrame = true; break;
			}
		}

		if (action == GLFW_RELEASE && key == GLFW_KEY_F2) {
			if (paused) {
				nextFrame = true;
			}
		}
	}


	static void tick(TickContext& tickContext, const TickContext::EntityMap& entityMap) {
		for (auto& entry : entityMap) {
			for (const auto& entity : entry.second) {
				entity->tick(tickContext);
			}
		}
	}


	static void tick(TickContext& tickContext) {
		tick(tickContext, tickContext.getOpaqueEntityMap());
		tick(tickContext, tickContext.getTransparentEntityMap());
		tickContext.updateEntities();
	}


	static void render(const TickContext::EntityMap& entityMap, const map<GLuint, Shader*>& shaderById) {
		for (auto& entry : entityMap) {
			if (entry.second.empty()) continue;

			GLuint shader = entry.first;

			if (shader > 0) {
				glUseProgram(shader);
				shaderById.at(shader)->setView(player->getCamera().getView());
			}
			

			for (const auto& entity : entry.second) {
				entity->draw();
			}
		}
	}


	static void render(GLFWwindow* window, TickContext& tickContext, const map<GLuint, Shader*>& shaderById) {
		render(tickContext.getOpaqueEntityMap(), shaderById);

		glEnable(GL_BLEND);
		glDepthMask(GL_FALSE);

		render(tickContext.getTransparentEntityMap(), shaderById);

		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);

		glfwSwapBuffers(window);
	}


	static void loop(GLFWwindow* window, TickContext& tickContext, const map<GLuint, Shader*>& shaderById) {
		ofstream fpsFile("/tmp/fps.log");
		fpsFile << std::setprecision(2) << std::fixed;

		float drawTimeSum = 0;
		uint32_t drawTimes = 0;

		for (float lastFrame = 0; !glfwWindowShouldClose(window);) {
			// update time
			float currentFrame = glfwGetTime();
			tickContext.deltaTime = currentFrame - lastFrame;
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
			render(window, tickContext, shaderById);
			float drawEndTime = glfwGetTime();

			// print fps
			float time = drawEndTime - drawStartTime;
			drawTimeSum += time;
			drawTimes++;

			fpsFile << std::setw(5) << time * 1000.0f << " ms, "
					<< std::setw(7) << 1.0f / time << " fps\n";


			// check paused
			if (paused) {
				while (paused && !nextFrame && !glfwWindowShouldClose(window)) {
					std::this_thread::sleep_for(std::chrono::milliseconds(100));
					glfwPollEvents();
				}

				nextFrame = false;
				lastFrame = glfwGetTime() - 1.0f / refreshRate;
			}
		}
		
		fpsFile << "Average: "
				<< std::setw(5) << 1000.0f * drawTimeSum / drawTimes << "ms, "
				<< std::setw(7) << drawTimes / drawTimeSum << " fps\n" << std::flush;
	}


	static void parse_args(int argc, const char* argv[]) {
		for (int i = 1; i < argc; i++) {
			std::string arg = argv[i];

			if (arg == "--lines") {
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
		}
	}
}

int main(int argc, const char* argv[]) {
	using namespace hack_game;

	srand(time(nullptr));

	GLFWwindow* window = initGLFW();
	initGL();

	parse_args(argc, argv);
	

	GLuint mainShaderId  = createShaderProgram("shaders/main.vert",  "shaders/main.frag");
	GLuint lightShaderId = createShaderProgram("shaders/light.vert", "shaders/light.frag");

	DrawContext drawContext {
		.nullShader               = Shader(),
		.mainShader               = Shader(mainShaderId),
		.lightShader              = Shader(lightShaderId),
		.shaders = {
			{ "enemyDamage",        Shader(createAnimationShaderProgram("animation.vert", "enemy-damage.frag"))         },
			{ "enemyDestroyCircle", Shader(createAnimationShaderProgram("animation.vert", "enemy-destroy-circle.frag")) },
			{ "enemyDestroySquare", Shader(createAnimationShaderProgram("animation.vert", "enemy-destroy-square.frag")) },
			{ "enemyDestroyCube",   Shader(createAnimationShaderProgram("animation.vert", "enemy-destroy-cube.frag"))   },
			{ "minionDestroy",      Shader(createAnimationShaderProgram("animation.vert", "minion-destroy.frag"))       },
		}
	};

	map<GLuint, Shader*> shaderById {
		{ -1,                         &drawContext.nullShader               },
		{ mainShaderId,               &drawContext.mainShader               },
		{ lightShaderId,              &drawContext.lightShader              },
	};

	for (auto& entry : drawContext.shaders) {
		shaderById[entry.second.id] = &entry.second;
	}


	initShaderUniforms(drawContext);


	TickContext tickContext	= createTickContext(drawContext);
	loop(window, tickContext, shaderById);

	glfwTerminate();
	return 0;
}
