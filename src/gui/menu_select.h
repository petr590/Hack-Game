#ifndef HACK_GAME__GUI__MENU_SELECT_H
#define HACK_GAME__GUI__MENU_SELECT_H

#include "element.h"
#include "gl_fwd.h"
#include <vector>

namespace hack_game {

	class Menu;

	class MenuSelect: public Element {
	public:
		class Selectable;

	private:
		Menu& menu;
		std::vector<Selectable> selectables;
		size_t selected = 0;
		const GLuint pointerTextureId;

	public:
		MenuSelect(Menu&, size_t count) noexcept;
		~MenuSelect() noexcept;

		void draw(const GuiContext&) override;
	
	private:
		void drawContent(const GuiContext&);
	};
}

#endif