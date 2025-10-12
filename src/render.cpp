#include "render.h"
#include "init.h"
#include "globals.h"
#include "context/shader.h"
#include "context/tick_context.h"
#include "entity/player.h"
#include "model/models.h"
#include "gui/menu.h"
#include "util.h"

#include <fstream>
#include <iomanip>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "imgui_util.h"

namespace hack_game {
	#define RGB(vec) vec.x, vec.y, vec.z
	#define RGBA(vec) vec.x, vec.y, vec.z, vec.w

	using std::map;
	using std::ofstream;

	using glm::vec3;

	static constexpr ImVec4 BACKGROUND         = colorAsImVec4(0xFF'636155);
	static constexpr ImVec4 ENDGAME_TEXT_COLOR = colorAsImVec4(0xFF'D0CBC1);

	static constexpr float ENDGAME_FADE_DURATION        = 0.25f;
	static constexpr float ENDGAME_TEXT_APPEAR_START    = 0.25f;
	static constexpr float ENDGAME_TEXT_APPEAR_DURATION = 0.25f;
	

	static void render(const TickContext& context, const TickContext::EntityMap& entityMap, const map<GLuint, Shader*>& shaderById) {
		for (auto& entry : entityMap) {
			if (entry.second.empty()) continue;

			GLuint shaderId = entry.first;

			if (shaderId > 0) {
				glUseProgram(shaderId);
				shaderById.at(shaderId)->setView(context.player->getCamera().getView());
			}
			

			for (const auto& entity : entry.second) {
				entity->draw();
			}
		}
	}


	static void render(const TickContext& context, const map<GLuint, Shader*>& shaderById) {
		render(context, context.getOpaqueEntityMap(), shaderById);

		glEnable(GL_BLEND);
		glDepthMask(GL_FALSE);

		render(context, context.getTransparentEntityMap(), shaderById);

		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);
	}


	void renderGL(const Initializer& initializer, const TickContext& tickContext, Shader& postprocessingShader, const map<GLuint, Shader*>& shaderById, ofstream& fpsFile, float endGameTime) {
		const int windowWidth = initializer.getWindowWidth();
		const int windowHeight = initializer.getWindowHeight();
		const FramebufferInfo& fbInfo = initializer.getFbInfo();

		const float renderStartTime = glfwGetTime();

		// render scene
		glBindFramebuffer(GL_FRAMEBUFFER, fbInfo.msFramebuffer);
		glClearColor(RGBA(BACKGROUND));
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glEnable(GL_MULTISAMPLE);
		
		render(tickContext, shaderById);

		const float bufferSwapTime = glfwGetTime();

		// blit
		glBindFramebuffer(GL_READ_FRAMEBUFFER, initializer.getFbInfo().msFramebuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, initializer.getFbInfo().framebuffer);
		glBlitFramebuffer(0, 0, windowWidth, windowHeight, 0, 0, windowWidth, windowHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);

		// postprocessing
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		glDisable(GL_MULTISAMPLE);

		postprocessingShader.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, fbInfo.texture);
		if (gameEnded()) {
			postprocessingShader.setUniform("progress", std::min(1.0f, endGameTime / ENDGAME_FADE_DURATION));
		}
		models::postprocessingModel.draw(postprocessingShader);
		glBindTexture(GL_TEXTURE_2D, 0);

		const float renderEndTime = glfwGetTime();

		// print fps
		fpsFile << std::setw(7)
				<< log2(1.0f / (renderEndTime  - renderStartTime)) << " fps, "
				<< log2(1.0f / (bufferSwapTime - renderStartTime)) << " fps (render), "
				<< log2(1.0f / (renderEndTime  - bufferSwapTime))  << " fps (postprocessing)\n";
	}


	static void renderEndGameScreen(float endGameTime) {
		ImGui::Begin("EndGame", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
		ImGui::SetWindowFontScale(1.33f);

		const float progress = std::min(1.0f, (endGameTime - ENDGAME_TEXT_APPEAR_START) / ENDGAME_TEXT_APPEAR_DURATION);
			
		const char* text = "HACKING COMPLETE";
		const ImVec2 size = ImGui::GetWindowSize();
		const ImVec2 textSize = ImGui::CalcTextSize(text);

		const float yOffset = std::pow(1.0f - progress, 2.0f) * textSize.y * 0.25f;

		ImGui::SetCursorPos(ImVec2(
			(size.x - textSize.x) * 0.5f,
			(size.y - textSize.y) * 0.5f + yOffset
		));

		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(RGB(ENDGAME_TEXT_COLOR), progress));
		ImGui::Text("%s", text);

		const ImVec2 lineStart(
			(size.x - textSize.x) * 0.5f,
			(size.y + textSize.y) * 0.5f + yOffset
		);

		const ImVec2 lineEnd(lineStart.x + textSize.x, lineStart.y);

		ImGui::GetWindowDrawList()->AddLine(lineStart, lineEnd, ImGui::ColorConvertFloat4ToU32(ENDGAME_TEXT_COLOR), 2.0f);
		ImGui::PopStyleColor();

		ImGui::End();
	}


	void renderImGui(GLFWwindow* window, float deltaTime, float endGameTime) {

		static GuiContext guiContext;
		static Menu menu(48);

		int fbWidth, fbHeight, winWidth, winHeight;
		glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
		glfwGetWindowSize(window, &winWidth, &winHeight);

		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(fbWidth, fbHeight);
		io.DisplayFramebufferScale = ImVec2(
				static_cast<float>(fbWidth) / winWidth,
				static_cast<float>(fbHeight) / winHeight
		);

		const ImVec2 scale = io.DisplaySize / NORMAL_WINDOW_SIZE;
		io.FontGlobalScale = std::min(scale.x, scale.y);

		ImGui::NewFrame();

		// ImGui::ShowDemoWindow();

		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(io.DisplaySize);

		guiContext.setDeltaTime(deltaTime);
		guiContext.setScale(scale);
		menu.draw(guiContext);

		// if (endGameTime >= ENDGAME_TEXT_APPEAR_START) {
		// 	renderEndGameScreen(endGameTime);
		// }

		ImGui::Render();
	}
}