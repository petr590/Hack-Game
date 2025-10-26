#include "menu.h"
#include "level/level.h"

namespace hack_game {

	static constexpr ImVec4 BG_COLOR   = colorAsImVec4(0xFF'C9C5A8);
	static constexpr ImVec4 TEXT_COLOR = colorAsImVec4(0xFF'454232);\

	std::shared_ptr<Player> Menu::getPlayer() const noexcept {
		return level != nullptr ? level->getPlayer() : nullptr;
	}

	void Menu::setDeltaTime(float deltaTime) const noexcept {
		if (level != nullptr) {
			level->setDeltaTime(deltaTime);
		}
	}

	void Menu::loadLevel(const std::string& path) {
		level = std::make_shared<Level>(shaderManager, path);
	}

	void Menu::draw(const GuiContext& context) {
		if (level != nullptr) return;

		ImGui::PushStyleColor(ImGuiCol_Text, TEXT_COLOR);
		ImGui::PushStyleColor(ImGuiCol_WindowBg, BG_COLOR);
		ImGui::PushStyleColor(ImGuiCol_ChildBg, PANEL_COLOR);
		ImGui::Begin("Menu", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

		select.draw(context);
		bottomPanel.draw(context);

		ImGui::End();
		ImGui::PopStyleColor(3);
	}
}