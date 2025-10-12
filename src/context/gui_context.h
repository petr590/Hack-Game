#ifndef HACK_GAME__CONTEXT__GUI_CONTEXT_H
#define HACK_GAME__CONTEXT__GUI_CONTEXT_H

#include "imgui_util.h"

namespace hack_game {

	class GuiContext {
		float deltaTime = 0;
		ImVec2 scale = {1, 1};
	
	public:
		float getDeltaTime() const noexcept {
			return deltaTime;
		}

		const ImVec2& getScale() const noexcept {
			return scale;
		}

		void setDeltaTime(float deltaTime) noexcept {
			this->deltaTime = deltaTime;
		}

		void setScale(const ImVec2& scale) noexcept {
			this->scale = scale;
		}

		float scaleX(float x) const noexcept {
			return x * scale.x;
		}

		float scaleY(float y) const noexcept {
			return y * scale.y;
		}

		ImVec2 scaleX(float x, float y) const noexcept {
			return ImVec2(x * scale.x, y);
		}

		ImVec2 scaleY(float x, float y) const noexcept {
			return ImVec2(x, y * scale.y);
		}

		ImVec2 scaleVec(const ImVec2& vec) const noexcept {
			return vec * scale;
		}

		ImVec2 scaleVec(float x, float y) const noexcept {
			return ImVec2(x, y) * scale;
		}
	};
}

#endif