#include "menu_select.h"
#include "imgui_util.h"
#include "menu.h"
#include <string>
#include <cmath>

namespace hack_game {

	using std::min;
	using std::clamp;
	using std::exp;
	using std::string;
	using std::to_string;

	using Selectable = MenuSelect::Selectable;


	static constexpr ImVec4 TRANSPARENT = {0, 0, 0, 0};

	static constexpr ImVec2 MARGIN = {65, 220};
	static constexpr ImVec2 SIZE   = {575, 575};

	static constexpr ImVec2 BAR_SIZE = {65, SIZE.y};
	static constexpr ImU32 BAR_COLOR        = colorAsImU32(0xFF'AEAA92);
	static constexpr ImU32 BAR_STRIPE_COLOR = colorAsImU32(0xFF'908B78);

	static constexpr ImVec2 PANEL_START = MARGIN;
	static constexpr ImVec2 PANEL_END = MARGIN + SIZE;


	static constexpr float SCROLLBAR_WIDTH     = 6;
	static constexpr float SCROLLBAR_PADDING_X = 21;

	static constexpr ImVec2 TOP_LEFT_PADDING = {BAR_SIZE.x, 20};
	static constexpr ImVec2 BOTTOM_RIGHT_PADDING = {SCROLLBAR_PADDING_X * 2 + SCROLLBAR_WIDTH, 21};

	static constexpr float STRIPE_HEIGHT           = 2;
	static constexpr float STRIPE_OFFSET_X         = 10;
	static constexpr float SELECTABLE_MARGIN_Y     = 6;
	static constexpr float SELECTABLE_PADDING_XY   = 5;
	static constexpr ImVec2 SELECTABLE_SQUARE_SIZE = {24, 24};

	static constexpr float SELECT_ANIMATION_TIME = 0.45f;
	static constexpr float SELECT_FADE_TIME      = 0.15f;


	class MenuSelect::Selectable {
		const string label;
		float time;
	
	public:
		Selectable(size_t num, bool isSelected):
				label("\t  Мини-игра по взлому #" + to_string(num)),
				time(isSelected ? 1.0f : 0.0f) {}
		

		bool draw(const GuiContext&, const ImVec2& region, bool isSelected);
	};


	MenuSelect::MenuSelect(size_t count) noexcept {
		selectables.reserve(count);

		for (size_t i = 0; i < count; i++) {
			selectables.emplace_back(i + 1, i == selected);
		}
	}

	MenuSelect::~MenuSelect() noexcept {}


	void MenuSelect::draw(const GuiContext& context) {

		ImDrawList* drawList = ImGui::GetWindowDrawList();

		constexpr float OFFSET = Menu::SHADOW_OFFSET;
		drawList->AddRectFilled(context.scaleVec(PANEL_START + OFFSET), context.scaleVec(PANEL_END + OFFSET), Menu::SHADOW_COLOR);
		drawList->AddRectFilled(context.scaleVec(PANEL_START),          context.scaleVec(PANEL_END),          Menu::PANEL_COLOR);

		drawList->AddRectFilled(context.scaleVec(PANEL_START), context.scaleVec(PANEL_START + BAR_SIZE), BAR_COLOR);

		drawList->AddRectFilled(
			context.scaleVec(PANEL_START.x + Menu::STRIPE1_START, PANEL_START.y),
			context.scaleVec(PANEL_START.x + Menu::STRIPE1_END,   PANEL_END.y),
			BAR_STRIPE_COLOR
		);

		drawList->AddRectFilled(
			context.scaleVec(PANEL_START.x + Menu::STRIPE2_START, PANEL_START.y),
			context.scaleVec(PANEL_START.x + Menu::STRIPE2_END,   PANEL_END.y),
			BAR_STRIPE_COLOR
		);

		drawContent(context);
	}

	static void drawScrollbar(const GuiContext& context, float regionY, float scrollY, float maxScrollY);
	static void drawDecorations(const GuiContext& context);

	void MenuSelect::drawContent(const GuiContext& context) {
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0)); // Убрать лишние отступы
		ImGui::PushStyleColor(ImGuiCol_Header,        TRANSPARENT);
		ImGui::PushStyleColor(ImGuiCol_HeaderHovered, TRANSPARENT);
		ImGui::PushStyleColor(ImGuiCol_HeaderActive,  TRANSPARENT);
		ImGui::PushStyleColor(ImGuiCol_NavHighlight,  TRANSPARENT);

		constexpr ImVec2 selectStart = PANEL_START + TOP_LEFT_PADDING;
		constexpr ImVec2 selectSize = PANEL_END - BOTTOM_RIGHT_PADDING - selectStart;

		ImGui::SetCursorPos(context.scaleVec(selectStart));
		ImGui::BeginChild("MenuSelectContent", context.scaleVec(selectSize), ImGuiChildFlags_None, ImGuiWindowFlags_NoScrollbar);

		ImGui::SetItemAllowOverlap(); // Отключить рамку при фокусе на Child

		const size_t count = selectables.size();

		if (ImGui::IsKeyPressed(ImGuiKey_UpArrow))   selected = (selected + count - 1) % count;
		if (ImGui::IsKeyPressed(ImGuiKey_DownArrow)) selected = (selected + 1) % count;

		const ImVec2 region = ImGui::GetContentRegionAvail();

		for (size_t i = 0; i < count; i++) {
			bool isSelected = selectables[i].draw(context, region, i == selected);

			if (isSelected) {
				selected = i;
			}
		}

		// Необходимо получить scrollY и maxScrollY до вызова EndChild.
		// Но скроллбар отрисовывается только после вызова EndChild, так как он за пределами текущего Child.
		const float scrollY = ImGui::GetScrollY();
		const float maxScrollY = ImGui::GetScrollMaxY();

		ImGui::EndChild();
		ImGui::PopStyleColor(4);
		ImGui::PopStyleVar();

		drawScrollbar(context, region.y, scrollY, maxScrollY);
		drawDecorations(context);
	}


	static void drawScrollbar(const GuiContext& context, float regionY, float scrollY, float maxScrollY) {
		const float scrollbarHeight = regionY * regionY / maxScrollY;
		const float scrollbarY = (regionY - scrollbarHeight) * scrollY / maxScrollY;

		const ImVec2 start = {
				PANEL_END.x - BOTTOM_RIGHT_PADDING.x + SCROLLBAR_PADDING_X,
				PANEL_START.y + TOP_LEFT_PADDING.y + scrollbarY
		};

		ImGui::GetWindowDrawList()->AddRectFilled(
				context.scaleVec(start),
				context.scaleVec(start + ImVec2(SCROLLBAR_WIDTH, scrollbarHeight)),
				Menu::SELECTION_COLOR
		);
	}


	static void drawDecorations(const GuiContext& context) {
		constexpr ImVec2 stripe1Start = PANEL_START + TOP_LEFT_PADDING * ImVec2(1.0f, 0.5f) + ImVec2(STRIPE_OFFSET_X, -0.5f * STRIPE_HEIGHT);
		constexpr ImVec2 stripe1Size = {PANEL_END.x - PANEL_START.x - TOP_LEFT_PADDING.x - BOTTOM_RIGHT_PADDING.x - STRIPE_OFFSET_X * 2, STRIPE_HEIGHT};
		constexpr ImVec2 stripe2Offset = {0, PANEL_END.y - PANEL_START.y - TOP_LEFT_PADDING.y};
		constexpr ImVec2 square1Start = {
				PANEL_END.x - BOTTOM_RIGHT_PADDING.x + SCROLLBAR_PADDING_X,
				stripe1Start.y - 0.5f * (SCROLLBAR_WIDTH - STRIPE_HEIGHT)
		};

		ImGui::GetWindowDrawList()->AddRectFilled( // stripe 1
				context.scaleVec(stripe1Start),
				context.scaleVec(stripe1Start + stripe1Size),
				BAR_STRIPE_COLOR
		);

		ImGui::GetWindowDrawList()->AddRectFilled( // stripe 2
				context.scaleVec(stripe1Start + stripe2Offset),
				context.scaleVec(stripe1Start + stripe2Offset + stripe1Size),
				BAR_STRIPE_COLOR
		);

		ImGui::GetWindowDrawList()->AddRectFilled( // square 1
				context.scaleVec(square1Start),
				context.scaleVec(square1Start + SCROLLBAR_WIDTH),
				BAR_STRIPE_COLOR
		);

		ImGui::GetWindowDrawList()->AddRectFilled( // square 2
				context.scaleVec(square1Start + stripe2Offset),
				context.scaleVec(square1Start + stripe2Offset + SCROLLBAR_WIDTH),
				BAR_STRIPE_COLOR
		);
	}


	static constexpr ImVec4 mix(const ImVec4& v1, const ImVec4& v2, float a) {
		return ImVec4(
			v1.x + (v2.x - v1.x) * a,
			v1.y + (v2.y - v1.y) * a,
			v1.z + (v2.z - v1.z) * a,
			v1.w + (v2.w - v1.w) * a
		);
	}

	static constexpr ImVec4 alpha(const ImVec4& color, float alpha) {
		return ImVec4(color.x, color.y, color.z, color.w * alpha);
	}


	static void drawAnimatedSelectable(const GuiContext& context, const ImVec2& region, bool isSelected, float time, const char* label);
	static bool drawPlainSelectable(const GuiContext& context, bool isSelected, const char* label);
	static void drawSquare(const GuiContext& context, float textHeight, const ImVec4& textColor);


	bool MenuSelect::Selectable::draw(const GuiContext& context, const ImVec2& region, bool isSelected) {
		if (isSelected) {
			time = min(time + context.getDeltaTime(), SELECT_ANIMATION_TIME);
		} else {
			time = clamp(time - context.getDeltaTime(), 0.0f, SELECT_FADE_TIME);
		}

		if (time > 0) {
			drawAnimatedSelectable(context, region, isSelected, time, label.c_str());
			return false;
		}

		return drawPlainSelectable(context, isSelected, label.c_str());
	}


	static void drawAnimatedSelectable(const GuiContext& context, const ImVec2& region, bool isSelected, float time, const char* label) {
		// variables
		const float progress = time / SELECT_ANIMATION_TIME;
		const float fadeProgress = clamp(time / SELECT_FADE_TIME, 0.0f, 1.0f);

		const ImVec4 stripeColor = alpha(ImGui::ColorConvertU32ToFloat4(Menu::SELECTION_COLOR), fadeProgress);
		const ImVec4 textColor = mix(ImGui::GetStyleColorVec4(ImGuiCol_Text), ImGui::ColorConvertU32ToFloat4(Menu::SELECTION_TEXT_COLOR), fadeProgress);

		const float rectX = isSelected ?
				(1.0f - exp(-6.0f * progress)) * (1.0f / (1.0f - exp(-6.0f))) : fadeProgress;

		const ImVec2 padding = context.scaleY(0, STRIPE_HEIGHT + SELECTABLE_MARGIN_Y);
		const ImVec2 stripeSize = context.scaleY(region.x, STRIPE_HEIGHT);
		const float textHeight = ImGui::CalcTextSize(label, nullptr, true).y;
		const ImVec2 rectSize = {
				rectX * (ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x),
				textHeight + context.scaleY(SELECTABLE_PADDING_XY * 2)
		};

		const float top = ImGui::GetCursorPosY();

		// draw
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		
		drawList->AddRectFilled( // stripe
				ImGui::GetCursorScreenPos(),
				ImGui::GetCursorScreenPos() + stripeSize,
				ImGui::ColorConvertFloat4ToU32(stripeColor)
		);

		ImGui::Dummy(padding);

		drawList->AddRectFilled( // rect
				ImGui::GetCursorScreenPos(),
				ImGui::GetCursorScreenPos() + rectSize,
				Menu::SELECTION_COLOR
		);

		ImGui::Dummy(context.scaleY(0, SELECTABLE_PADDING_XY));

		ImGui::PushStyleColor(ImGuiCol_Text, textColor);
		ImGui::Selectable(label, true);
		ImGui::PopStyleColor();

		drawSquare(context, textHeight, textColor);

		const float scrollY = ImGui::GetScrollY();
		const float height = rectSize.y + padding.y * 2;
		
		if (top < scrollY) {
			ImGui::SetScrollY(top);
		} else if (top + height > scrollY + region.y) {
			ImGui::SetScrollY(top - region.y + height);
		}

		ImGui::Dummy(padding + context.scaleY(0, SELECTABLE_PADDING_XY));

		const ImVec2 stripeStart = ImGui::GetCursorScreenPos() - ImVec2(0, stripeSize.y);
		drawList->AddRectFilled( // stripe
				stripeStart,
				stripeStart + stripeSize,
				ImGui::ColorConvertFloat4ToU32(stripeColor)
		);
	}


	static bool drawPlainSelectable(const GuiContext& context, bool isSelected, const char* label) {
		const ImVec2 padding = context.scaleY(0, STRIPE_HEIGHT + SELECTABLE_MARGIN_Y + SELECTABLE_PADDING_XY);

		ImGui::Dummy(padding);

		bool clicked = ImGui::Selectable(label, false);

		if (clicked || ImGui::IsItemHovered()) {
			isSelected = true;
		}

		drawSquare(context, ImGui::CalcTextSize(label, nullptr, true).y, ImGui::GetStyleColorVec4(ImGuiCol_Text));

		ImGui::Dummy(padding);
		return isSelected;
	}


	static void drawSquare(const GuiContext& context, float textHeight, const ImVec4& textColor) {
		const float textHeightScaledX = context.scaleX(textHeight / context.getScale().y);
		const ImVec2 squarePadding = (ImVec2(textHeightScaledX, textHeight) - context.scaleVec(SELECTABLE_SQUARE_SIZE)) * 0.5f;
		const ImVec2 squareStart = ImGui::GetCursorScreenPos() + squarePadding + context.scaleX(SELECTABLE_PADDING_XY, -textHeight);

		ImGui::GetWindowDrawList()->AddRectFilled(
			squareStart,
			squareStart + context.scaleVec(SELECTABLE_SQUARE_SIZE),
			ImGui::ColorConvertFloat4ToU32(textColor)
		);
	}
}