#include "main.h"
#include "init.h"
#include "render.h"
#include "shader/shader_manager.h"
#include "entity/player.h"
#include "gui/menu.h"

#include <fstream>
#include <iomanip>
#include <thread>
#include <chrono>

#include <GLFW/glfw3.h>
#include "nowarn_imgui.h"
#include "nowarn_imgui_backends.h"

namespace hack_game {

	using std::unique_ptr;
	using std::shared_ptr;
	using std::make_unique;
	using std::ostream;
	using std::ofstream;

	static ShaderManager* staticDrawContext = nullptr;
	static volatile bool paused = false;
	static volatile bool nextFrame = false;


	void onChangeWindowSize(GLint width, GLint height) {
		staticDrawContext->updateWindowSize(width, height);
	}

	void updateKeys(GLFWwindow* window, const shared_ptr<Player>& player) {
		if (player != nullptr) {
			player->updateKeys();
		}

		if (ImGui::IsKeyPressed(ImGuiKey_Escape)) glfwSetWindowShouldClose(window, GL_TRUE);
		if (ImGui::IsKeyPressed(ImGuiKey_F1)) {
			paused = !paused;

			auto& data = ImGui::GetIO().KeysData[ImGuiKey_F1 - ImGuiKey_NamedKey_BEGIN];
			data.Down = false;
			data.DownDuration = -1.0f;
			data.DownDurationPrev = -1.0f;
		}

		if (ImGui::IsKeyPressed(ImGuiKey_F2) && paused) nextFrame = true;
	}


	static void tick(Level& level, const Level::EntityMap& entityMap) {
		for (auto& entry : entityMap) {
			for (const auto& entity : entry.second) {
				entity->tick(level);
			}
		}
	}


	static void tick(Level& level) {
		tick(level, level.getOpaqueEntityMap());
		tick(level, level.getTransparentEntityMap());
		level.updateEntities();
	}


	void mainLoop(const WindowData& windowData, ShaderManager& shaderManager, bool profile) {
		static Menu menu(shaderManager, 48);

		GLFWwindow* const window = windowData.getWindow();

		staticDrawContext = &shaderManager;

		unique_ptr<ostream> fpsFile = nullptr;

		if (profile) {
			fpsFile = make_unique<ofstream>("/tmp/fps.log");
			*fpsFile << std::fixed << std::setprecision(2) << std::setw(7);
		}

		const float waitTime = 1.0f / windowData.getRefreshRate();
		float endGameTime = 0;

		for (float lastFrame = 0; !glfwWindowShouldClose(window);) {
			// update time
			const float currentFrame = glfwGetTime();
			const float deltaTime = currentFrame - lastFrame;
			menu.setDeltaTime(deltaTime);
			lastFrame = currentFrame;

			// tick
			glfwPollEvents();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			updateKeys(window, menu.getPlayer());

			if (menu.getLevel() != nullptr) {
				tick(*menu.getLevel().get());
			}

			render(windowData, shaderManager, menu, fpsFile, deltaTime, endGameTime);
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			glfwSwapBuffers(window);

			// check paused
			if (paused) {
				while (paused && !nextFrame && !glfwWindowShouldClose(window)) {
					std::this_thread::sleep_for(std::chrono::nanoseconds(static_cast<uint64_t>(waitTime * (1e9f / 2))));
					glfwPollEvents();
					ImGui_ImplOpenGL3_NewFrame();
					ImGui_ImplGlfw_NewFrame();
					ImGui::NewFrame();
					updateKeys(window, menu.getPlayer());
					ImGui::Render();
					ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
				}

				nextFrame = false;
				lastFrame = glfwGetTime() - waitTime;
			}
		}
	}
}