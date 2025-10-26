#include "imgui_util.h"

namespace hack_game {

	ImVec2 globalScale = {0, 0};

	void AddRectFilledScaled(const ImVec2& pMin, const ImVec2& pMax, ImU32 color, float rounding, ImDrawFlags flags) {
		ImGui::GetWindowDrawList()->AddRectFilled(
				pMin * globalScale,
				pMax * globalScale,
				color, rounding, flags
		);
	}
}