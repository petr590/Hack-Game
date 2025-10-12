#ifndef HACK_GAME__GUI__ELEMENT_H
#define HACK_GAME__GUI__ELEMENT_H

#include "context/gui_context.h"

namespace hack_game {

	static inline constexpr ImVec2 NORMAL_WINDOW_SIZE = {1920, 1025};

	class Element {
	public:
		virtual ~Element() noexcept = default;
		virtual void draw(const GuiContext&) = 0;
	};
}

#endif