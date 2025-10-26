#ifndef HACK_GAME__GUI__MENU_H
#define HACK_GAME__GUI__MENU_H

#include "menu_bottom_panel.h"
#include "menu_select.h"
#include "imgui_util.h"
#include <string>
#include <memory>

namespace hack_game {

	class Player;
	class Level;
	class ShaderManager;

	class Menu: public Element {
		ShaderManager& shaderManager;
		MenuBottomPanel bottomPanel;
		MenuSelect select;
		std::shared_ptr<Level> level = nullptr;

	public:
		static constexpr float SHADOW_OFFSET        = 4;
		static constexpr ImU32 SHADOW_COLOR         = colorAsImU32(0x3F'000000);
		static constexpr ImU32 PANEL_COLOR          = colorAsImU32(0xFF'D5CFAE);
		static constexpr ImU32 SELECTION_COLOR      = colorAsImU32(0xFF'4A4840);
		static constexpr ImU32 SELECTION_TEXT_COLOR = PANEL_COLOR;

		static constexpr float STRIPE1_START = 0;
		static constexpr float STRIPE1_END   = 15 + STRIPE1_START;
		static constexpr float STRIPE2_START = 7 + STRIPE1_END;
		static constexpr float STRIPE2_END   = 3 + STRIPE2_START;

		Menu(ShaderManager& shaderManager, size_t levelsCount) noexcept:
				shaderManager(shaderManager), select(*this, levelsCount) {}

		const std::shared_ptr<Level>& getLevel() const noexcept {
			return level;
		}

		std::shared_ptr<Player> getPlayer() const noexcept;

		void setDeltaTime(float) const noexcept;
		void loadLevel(const std::string& path);
		void draw(const GuiContext&) override;
	};
}

#endif