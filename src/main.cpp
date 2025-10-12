#include "main.h"
#include "init.h"
#include "globals.h"
#include "entity/player.h"
#include "render.h"
#include "scancodes.h"
#include "util.h"

#include <fstream>
#include <iomanip>
#include <thread>
#include <chrono>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "nowarn_imgui.h"
#include "nowarn_imgui_backends.h"

namespace hack_game {

	using std::map;
	using std::ofstream;
	using std::shared_ptr;

	using glm::vec2;
	using glm::vec3;
	
	bool enemyDestroyed = false;
	bool playerDestroyed = false;
	int destroyAnimationCount = 0;

	static shared_ptr<Player> staticPlayer = nullptr;
	static Shader* staticPostprocessingShader = nullptr;

	static volatile bool paused = false;
	static volatile bool nextFrame = false;


	void onChangeWindowSize(int width, int height) {
		staticPostprocessingShader->setUniform("pixelSize", vec2(1.0f / width, 1.0f / height));
	}


	void updateKeys(GLFWwindow* window) {
		staticPlayer->updateKeys();

		if (ImGui::IsKeyPressed(ImGuiKey_Escape)) glfwSetWindowShouldClose(window, GL_TRUE);
		if (ImGui::IsKeyPressed(ImGuiKey_F1))           paused = !paused;
		if (ImGui::IsKeyPressed(ImGuiKey_F2) && paused) nextFrame = true;
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


	void mainLoop(const Initializer& initializer, Shader& postprocessingShader, TickContext& tickContext, const map<GLuint, Shader*>& shaderById) {
		const int windowWidth = initializer.getWindowWidth();
		const int windowHeight = initializer.getWindowHeight();
		GLFWwindow* window = initializer.getWindow();

		ofstream fpsFile("/tmp/fps.log");
		fpsFile << std::setprecision(2) << std::fixed;

		staticPlayer = tickContext.player;
		staticPostprocessingShader = &postprocessingShader;
		
		postprocessingShader.setUniform("screenTexture", 0);
		postprocessingShader.setUniform("progress", 0.0f);
		postprocessingShader.setUniform("pixelSize", vec2(1.0f / windowWidth, 1.0f / windowHeight));
		float endGameTime = 0;

		for (float lastFrame = 0; !glfwWindowShouldClose(window);) {
			// update time
			float currentFrame = glfwGetTime();
			tickContext.setDeltaTime(currentFrame - lastFrame);
			lastFrame = currentFrame;

			if (gameEnded()) {
				endGameTime += tickContext.getDeltaTime();
			}

			// tick
			glfwPollEvents();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			updateKeys(window);

			tick(tickContext);

			renderGL(initializer, tickContext, postprocessingShader, shaderById, fpsFile, endGameTime);
			renderImGui(window, tickContext.getDeltaTime(), endGameTime);
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			glfwSwapBuffers(window);

			// check paused
			if (paused) {
				while (paused && !nextFrame && !glfwWindowShouldClose(window)) {
					std::this_thread::sleep_for(std::chrono::milliseconds(100));
					glfwPollEvents();
				}

				nextFrame = false;
				lastFrame = glfwGetTime() - 1.0f / initializer.getRefreshRate();
			}
		}
		
		fpsFile << std::flush;
	}
}