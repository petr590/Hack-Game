#ifndef HACK_GAME__IMGUI_UTIL_H
#define HACK_GAME__IMGUI_UTIL_H

#include "nowarn_imgui.h"
#include <cstdint>

namespace hack_game {

	static constexpr ImVec2 operator+(const ImVec2& vec1, const ImVec2& vec2) noexcept { return ImVec2(vec1.x + vec2.x, vec1.y + vec2.y); }
	static constexpr ImVec2 operator-(const ImVec2& vec1, const ImVec2& vec2) noexcept { return ImVec2(vec1.x - vec2.x, vec1.y - vec2.y); }
	static constexpr ImVec2 operator*(const ImVec2& vec1, const ImVec2& vec2) noexcept { return ImVec2(vec1.x * vec2.x, vec1.y * vec2.y); }
	static constexpr ImVec2 operator/(const ImVec2& vec1, const ImVec2& vec2) noexcept { return ImVec2(vec1.x / vec2.x, vec1.y / vec2.y); }

	static constexpr ImVec2 operator+(const ImVec2& vec, float scalar) noexcept { return vec + ImVec2(scalar, scalar); }
	static constexpr ImVec2 operator-(const ImVec2& vec, float scalar) noexcept { return vec - ImVec2(scalar, scalar); }
	static constexpr ImVec2 operator*(const ImVec2& vec, float scalar) noexcept { return vec * ImVec2(scalar, scalar); }
	static constexpr ImVec2 operator/(const ImVec2& vec, float scalar) noexcept { return vec / ImVec2(scalar, scalar); }

	static constexpr ImVec2 operator+(float scalar, const ImVec2& vec) noexcept { return ImVec2(scalar, scalar) + vec; }
	static constexpr ImVec2 operator-(float scalar, const ImVec2& vec) noexcept { return ImVec2(scalar, scalar) - vec; }
	static constexpr ImVec2 operator*(float scalar, const ImVec2& vec) noexcept { return ImVec2(scalar, scalar) * vec; }
	static constexpr ImVec2 operator/(float scalar, const ImVec2& vec) noexcept { return ImVec2(scalar, scalar) / vec; }


	static constexpr ImU32 colorAsImU32(uint32_t argb) {
		return  ((static_cast<ImU32>(argb >> 16) & 0xFF) << IM_COL32_R_SHIFT) |
				((static_cast<ImU32>(argb >>  8) & 0xFF) << IM_COL32_G_SHIFT) |
				((static_cast<ImU32>(argb >>  0) & 0xFF) << IM_COL32_B_SHIFT) |
				((static_cast<ImU32>(argb >> 24) & 0xFF) << IM_COL32_A_SHIFT);
	}

	static constexpr ImVec4 colorAsImVec4(uint32_t argb) {
		return ImVec4(
			((argb >> 16) & 0xFF) * (1.f / 255.f),
			((argb >>  8) & 0xFF) * (1.f / 255.f),
			((argb      ) & 0xFF) * (1.f / 255.f),
			((argb >> 24) & 0xFF) * (1.f / 255.f)
		);
	}
}

#endif