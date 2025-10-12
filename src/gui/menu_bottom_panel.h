#ifndef HACK_GAME__GUI__MENU_BOTTOM_PANEL_H
#define HACK_GAME__GUI__MENU_BOTTOM_PANEL_H

#include "element.h"

namespace hack_game {

	class MenuBottomPanel: public Element {
	public:
		void draw(const GuiContext&) override;
	};
}

#endif