#include "menu.h"

namespace hack_game {

	static constexpr ImVec4 BG_COLOR   = colorAsImVec4(0xFF'C9C5A8);
	static constexpr ImVec4 TEXT_COLOR = colorAsImVec4(0xFF'454232);

	void Menu::draw(const GuiContext& context) {
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