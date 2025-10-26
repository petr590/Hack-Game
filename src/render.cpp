#include "render.h"
#include "init.h"
#include "globals.h"
#include "shader/shader_manager.h"
#include "entity/player.h"
#include "model/models.h"
#include "gui/menu.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "imgui_util.h"

namespace hack_game {

	#define RGB(vec) (vec).x, (vec).y, (vec).z
	#define RGBA(vec) (vec).x, (vec).y, (vec).z, (vec).w

	using std::min;
	using std::pow;
	using std::ostream;
	using std::unique_ptr;

	static constexpr ImVec4 BACKGROUND         = colorAsImVec4(0xFF'636155);
	static constexpr ImVec4 ENDGAME_TEXT_COLOR = colorAsImVec4(0xFF'D0CBC1);

	static constexpr float ENDGAME_FADE_DURATION        = 0.25f;
	static constexpr float ENDGAME_TEXT_APPEAR_START    = 0.25f;
	static constexpr float ENDGAME_TEXT_APPEAR_DURATION = 0.25f;


	static void renderGL(const WindowData& windowData, ShaderManager& shaderManager, const Level& level, const unique_ptr<ostream>& fpsFile, float endGameTime);
	static void renderImGui(GLFWwindow* window, Menu& menu, float deltaTime, float endGameTime);


	void render(const WindowData& windowData, ShaderManager& shaderManager, Menu& menu, const unique_ptr<ostream>& fpsFile, float deltaTime, float endGameTime) {
		if (menu.getLevel() != nullptr) {
			renderGL(windowData, shaderManager, *menu.getLevel().get(), fpsFile, endGameTime);
		}

		renderImGui(windowData.getWindow(), menu, deltaTime, endGameTime);
	}



	static void renderEntities(ShaderManager& shaderManager, const Level& level, const Level::EntityMap& entityMap) {
		for (auto& entry : entityMap) {
			if (entry.second.empty()) continue;

			const GLuint shaderId = entry.first;

			if (shaderId > 0) {
				glUseProgram(shaderId);
				shaderManager.getShader(shaderId).setView(level.getPlayer()->getCamera().getView());
			}
			

			for (const auto& entity : entry.second) {
				entity->draw();
			}
		}
	}


	static void renderScene(const WindowData& windowData, ShaderManager& shaderManager, const Level& level) {
		glBindFramebuffer(GL_FRAMEBUFFER, windowData.getFbInfo().msFramebuffer);
		glClearColor(RGBA(BACKGROUND));
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glEnable(GL_MULTISAMPLE);
				
		renderEntities(shaderManager, level, level.getOpaqueEntityMap());

		glEnable(GL_BLEND);
		glDepthMask(GL_FALSE);

		renderEntities(shaderManager, level, level.getTransparentEntityMap());

		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);
	}


	static void renderPostprocess(const WindowData& windowData, ShaderManager& shaderManager, float endGameTime) {
		const float windowWidth = windowData.getWindowWidth();
		const float windowHeight = windowData.getWindowHeight();

		glBindFramebuffer(GL_READ_FRAMEBUFFER, windowData.getFbInfo().msFramebuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, windowData.getFbInfo().framebuffer);
		glBlitFramebuffer(0, 0, windowWidth, windowHeight, 0, 0, windowWidth, windowHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		glDisable(GL_MULTISAMPLE);

		Shader& postprocessing = shaderManager.getShader("postprocessing");
		postprocessing.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, windowData.getFbInfo().texture);
		if (gameEnded()) {
			postprocessing.setUniform("progress", min(1.0f, endGameTime / ENDGAME_FADE_DURATION));
		}
		models::postprocessingModel.draw(postprocessing);
		glBindTexture(GL_TEXTURE_2D, 0);
	}


	static void renderGL(const WindowData& windowData, ShaderManager& shaderManager, const Level& level, const unique_ptr<ostream>& fpsFile, float endGameTime) {
		if (fpsFile == nullptr) {
			renderScene(windowData, shaderManager, level);
			renderPostprocess(windowData, shaderManager, endGameTime);

		} else {
			const float renderStartTime = glfwGetTime();

			renderScene(windowData, shaderManager, level);
			glFinish();

			const float blitTime = glfwGetTime();

			renderPostprocess(windowData, shaderManager, endGameTime);
			glFinish();
			const float renderEndTime = glfwGetTime();

			// print fps
			*fpsFile << (1.0f / (renderEndTime  - renderStartTime)) << " fps, "
					 << (1.0f / (blitTime       - renderStartTime)) << " fps (render), "
					 << (1.0f / (renderEndTime  - blitTime))        << " fps (postprocessing)\n";
		}
	}



	static void renderImGui(GLFWwindow* window, Menu& menu, float deltaTime, float endGameTime) {
		static GuiContext guiContext;
			
		int fbWidth, fbHeight, winWidth, winHeight;
		glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
		glfwGetWindowSize(window, &winWidth, &winHeight);

		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(fbWidth, fbHeight);
		io.DisplayFramebufferScale = ImVec2(
				static_cast<float>(fbWidth) / winWidth,
				static_cast<float>(fbHeight) / winHeight
		);

		globalScale = io.DisplaySize / NORMAL_WINDOW_SIZE;
		io.FontGlobalScale = min(globalScale.x, globalScale.y);

		ImGui::NewFrame();
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(io.DisplaySize);

		guiContext.setDeltaTime(deltaTime);
		guiContext.setScale(globalScale);

		menu.draw(guiContext);

		// if (endGameTime >= ENDGAME_TEXT_APPEAR_START) {
		// 	renderEndGameScreen(endGameTime);
		// }

		ImGui::Render();
	}


	static void renderEndGameScreen(float endGameTime) {
		ImGui::Begin("EndGame", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
		ImGui::SetWindowFontScale(1.33f);

		const float progress = min(1.0f, (endGameTime - ENDGAME_TEXT_APPEAR_START) / ENDGAME_TEXT_APPEAR_DURATION);
		
		const char* text = "HACKING COMPLETE";
		const ImVec2 size = ImGui::GetWindowSize();
		const ImVec2 textSize = ImGui::CalcTextSize(text);

		const float yOffset = pow(1.0f - progress, 2.0f) * textSize.y * 0.25f;

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
}