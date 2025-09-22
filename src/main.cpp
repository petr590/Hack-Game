#include "main.h"
#include "entity/player.h"
#include "model/models.h"
#include "util.h"
#include "scancodes.h"

#include <fstream>
#include <iomanip>
#include <thread>
#include <chrono>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace hack_game {

	using std::map;
	using std::ofstream;
	using std::shared_ptr;

	using glm::vec2;
	using glm::vec3;

	static constexpr vec3 BACKGROUND = colorAsVec3(0x636155);
	static constexpr float ENDGAME_DURATION = 1.0f;
	
	bool enemyDestroyed = false;
	bool playerDestroyed = false;
	int destroyAnimationCount = 0;

	static FramebufferInfo staticFbInfo;
	static shared_ptr<Player> staticPlayer = nullptr;
	static Shader* staticPostprocessingShader = nullptr;

	static bool paused = false;
	static bool nextFrame = false;


	static bool gameEnded() {
		return (playerDestroyed || enemyDestroyed) && destroyAnimationCount == 0;
	}


	void framebufferSizeCallback(GLFWwindow*, int width, int height) {
		windowWidth = width;
		windowHeight = height;

		staticPostprocessingShader->setPixelSize(vec2(1.0f / width, 1.0f / height));

		changeFramebufferSize(staticFbInfo);
	}


	void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		// Suppress compiler's warnings
		(void)mods;

		staticPlayer->onKey(scancode, action);

		if (key == GLFW_KEY_UNKNOWN) {
			switch (scancode) {
				case SCANCODE_ESCAPE: key = GLFW_KEY_ESCAPE; break;
				case SCANCODE_F1:     key = GLFW_KEY_F1;     break;
				case SCANCODE_F2:     key = GLFW_KEY_F2;     break;
			}
		}

		if (action == GLFW_PRESS) {
			switch (key) {
				case GLFW_KEY_ESCAPE: glfwSetWindowShouldClose(window, GL_TRUE); break;
				case GLFW_KEY_F1: paused = !paused; break;
				case GLFW_KEY_F2: if (paused) nextFrame = true; break;
			}
		}

		if (action == GLFW_REPEAT && key == GLFW_KEY_F2 && paused) {
			nextFrame = true;
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


	static void render(const TickContext& context, const TickContext::EntityMap& entityMap, const map<GLuint, Shader*>& shaderById) {
		for (auto& entry : entityMap) {
			if (entry.second.empty()) continue;

			GLuint shader = entry.first;

			if (shader > 0) {
				glUseProgram(shader);
				shaderById.at(shader)->setView(context.player->getCamera().getView());
			}
			

			for (const auto& entity : entry.second) {
				entity->draw();
			}
		}
	}


	static void render(TickContext& context, const map<GLuint, Shader*>& shaderById) {
		render(context, context.getOpaqueEntityMap(), shaderById);

		glEnable(GL_BLEND);
		glDepthMask(GL_FALSE);

		render(context, context.getTransparentEntityMap(), shaderById);

		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);
	}


	void mainLoop(GLFWwindow* const window, const FramebufferInfo& fbInfo, Shader& postprocessingShader, TickContext& tickContext, const map<GLuint, Shader*>& shaderById) {
		ofstream fpsFile("/tmp/fps.log");
		fpsFile << std::setprecision(2) << std::fixed;

		staticFbInfo = fbInfo;
		staticPlayer = tickContext.player;
		staticPostprocessingShader = &postprocessingShader;
		
		postprocessingShader.setProgress(0.0f);
		postprocessingShader.setPixelSize(vec2(1.0f / windowWidth, 1.0f / windowHeight));
		float endGameTime = 0;

		for (float lastFrame = 0; !glfwWindowShouldClose(window);) {
			// update time
			float currentFrame = glfwGetTime();
			tickContext.deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			// tick
			glfwPollEvents();
			tick(tickContext);

			const float renderStartTime = glfwGetTime();

			// render scene
			glBindFramebuffer(GL_FRAMEBUFFER, fbInfo.msFramebuffer);
			glClearColor(BACKGROUND.r, BACKGROUND.g, BACKGROUND.b, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_CULL_FACE);
			glEnable(GL_MULTISAMPLE);
			
			render(tickContext, shaderById);

			const float bufferSwapTime = glfwGetTime();

			// blit
			glBindFramebuffer(GL_READ_FRAMEBUFFER, fbInfo.msFramebuffer);
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbInfo.framebuffer);
			glBlitFramebuffer(0, 0, windowWidth, windowHeight, 0, 0, windowWidth, windowHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);

			// postprocessing
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			glDisable(GL_DEPTH_TEST);
			glDisable(GL_CULL_FACE);
			glDisable(GL_MULTISAMPLE);

			glUseProgram(postprocessingShader.getId());
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, fbInfo.texture);
			if (gameEnded()) {
				endGameTime += tickContext.deltaTime;
				postprocessingShader.setProgress(std::min(1.0f, endGameTime / ENDGAME_DURATION));
			}
			models::postprocessingModel.draw(postprocessingShader);
			glBindTexture(GL_TEXTURE_2D, 0);

			glfwSwapBuffers(window);

			const float renderEndTime = glfwGetTime();

			// print fps
			fpsFile << std::setw(7)
					<< 1.0f / (renderEndTime - renderStartTime) << " fps, "
					<< 1.0f / (bufferSwapTime - renderStartTime) << " fps (render), "
					<< 1.0f / (renderEndTime - bufferSwapTime) << " fps (postprocessing)\n";


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
		
		fpsFile << std::flush;
	}
}