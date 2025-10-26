#include "menu_select.h"
#include "imgui_util.h"
#include "menu.h"
#include "texture.h"
#include "dir_paths.h"
#include <string>
#include <cmath>

namespace hack_game {

	using std::min;
	using std::clamp;
	using std::exp;
	using std::string;
	using std::to_string;

	using Selectable = MenuSelect::Selectable;

	#define TEXTURE "pointer.png"


	static constexpr ImVec4 TRANSPARENT = {0, 0, 0, 0};

	static constexpr ImVec2 MARGIN = {65, 220};
	static constexpr ImVec2 SIZE   = {575, 575};

	static constexpr ImVec2 BAR_SIZE = {65, SIZE.y};
	static constexpr ImU32 BAR_COLOR        = colorAsImU32(0xFF'AEAA92);
	static constexpr ImU32 BAR_STRIPE_COLOR = colorAsImU32(0xFF'908B78);

	static constexpr ImVec2 POINTER_TEXTURE_SIZE = {45, 45};
	static constexpr float POINTER_TEXTURE_OFFSET_X = 4;

	static constexpr ImVec2 PANEL_START = MARGIN;
	static constexpr ImVec2 PANEL_END = MARGIN + SIZE;


	static constexpr float SCROLLBAR_WIDTH     = 6;
	static constexpr float SCROLLBAR_PADDING_X = 21;

	static constexpr ImVec2 TOP_LEFT_PADDING = {0, 20};
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
		const string path;
		float time;
	
	public:
		Selectable(size_t num, bool isSelected):
				label("\t  Мини-игра по взлому #" + to_string(num)),
				path(string(LEVELS_DIR "level") + to_string(num) + ".json"),
				time(isSelected ? 1.0f : 0.0f) {}
		
		const string& getLabel() const noexcept { return label; }
		const string& getPath() const noexcept { return path; }
		float getTime() const noexcept { return time; }

		bool draw(const GuiContext&, Menu&, const ImVec2& region, GLuint pointerTextureId, bool isSelected);
	};


	MenuSelect::MenuSelect(Menu& menu, size_t count) noexcept:
			menu(menu),
			pointerTextureId(Texture(TEXTURES_DIR TEXTURE).genGlTexture()) {
		
		selectables.reserve(count);

		for (size_t i = 0; i < count; i++) {
			selectables.emplace_back(i + 1, i == selected);
		}
	}

	MenuSelect::~MenuSelect() noexcept {}


	void MenuSelect::draw(const GuiContext& context) {

		constexpr float OFFSET = Menu::SHADOW_OFFSET;
		AddRectFilledScaled(PANEL_START + OFFSET, PANEL_END + OFFSET, Menu::SHADOW_COLOR);
		AddRectFilledScaled(PANEL_START,          PANEL_END,          Menu::PANEL_COLOR);

		drawContent(context);
	}

	static void drawScrollbar(float regionY, float scrollY, float maxScrollY);
	static void drawDecorations();

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

		ImGui::Indent(context.scaleX(BAR_SIZE.x));
		ImGui::SetItemAllowOverlap(); // Отключить рамку при фокусе на Child

		const size_t count = selectables.size();

		if (ImGui::IsKeyPressed(ImGuiKey_UpArrow))   selected = (selected + count - 1) % count;
		if (ImGui::IsKeyPressed(ImGuiKey_DownArrow)) selected = (selected + 1) % count;

		const ImVec2 region = ImGui::GetContentRegionAvail();

		for (size_t i = 0; i < count; i++) {
			bool isSelected = selectables[i].draw(context, menu, region, pointerTextureId, i == selected);

			if (isSelected) {
				selected = i;
			}
		}

		// Необходимо получить scrollY и maxScrollY до вызова EndChild.
		// Но скроллбар отрисовывается только после вызова EndChild, так как он за пределами текущего Child.
		const float scrollY = ImGui::GetScrollY();
		const float maxScrollY = ImGui::GetScrollMaxY();

		ImGui::Unindent(context.scaleX(BAR_SIZE.x));
		ImGui::EndChild();
		ImGui::PopStyleColor(4);
		ImGui::PopStyleVar();

		drawScrollbar(region.y, scrollY, maxScrollY);
		drawDecorations();
	}


	static void drawScrollbar(float regionY, float scrollY, float maxScrollY) {
		const float scrollbarHeight = regionY * regionY / maxScrollY;
		const float scrollbarY = (regionY - scrollbarHeight) * scrollY / maxScrollY;

		const ImVec2 start = {
				PANEL_END.x - BOTTOM_RIGHT_PADDING.x + SCROLLBAR_PADDING_X,
				PANEL_START.y + TOP_LEFT_PADDING.y + scrollbarY
		};

		AddRectFilledScaled(
				start,
				start + ImVec2(SCROLLBAR_WIDTH, scrollbarHeight),
				Menu::SELECTION_COLOR
		);
	}


	static void drawDecorations() {
		constexpr ImVec2 stripe1Start = PANEL_START + TOP_LEFT_PADDING * ImVec2(1.0f, 0.5f) + ImVec2(STRIPE_OFFSET_X, -0.5f * STRIPE_HEIGHT);
		constexpr ImVec2 stripe1Size = {PANEL_END.x - PANEL_START.x - TOP_LEFT_PADDING.x - BOTTOM_RIGHT_PADDING.x - STRIPE_OFFSET_X * 2, STRIPE_HEIGHT};
		constexpr ImVec2 stripe2Offset = {0, PANEL_END.y - PANEL_START.y - TOP_LEFT_PADDING.y};
		constexpr ImVec2 square1Start = {
				PANEL_END.x - BOTTOM_RIGHT_PADDING.x + SCROLLBAR_PADDING_X,
				stripe1Start.y - 0.5f * (SCROLLBAR_WIDTH - STRIPE_HEIGHT)
		};

		AddRectFilledScaled( // stripe 1
				stripe1Start,
				stripe1Start + stripe1Size,
				BAR_STRIPE_COLOR
		);

		AddRectFilledScaled( // stripe 2
				stripe1Start + stripe2Offset,
				stripe1Start + stripe2Offset + stripe1Size,
				BAR_STRIPE_COLOR
		);

		AddRectFilledScaled( // square 1
				square1Start,
				square1Start + SCROLLBAR_WIDTH,
				BAR_STRIPE_COLOR
		);

		AddRectFilledScaled( // square 2
				square1Start + stripe2Offset,
				square1Start + stripe2Offset + SCROLLBAR_WIDTH,
				BAR_STRIPE_COLOR
		);


		AddRectFilledScaled(PANEL_START, PANEL_START + BAR_SIZE, BAR_COLOR);

		AddRectFilledScaled(
			ImVec2(PANEL_START.x + Menu::STRIPE1_START, PANEL_START.y),
			ImVec2(PANEL_START.x + Menu::STRIPE1_END,   PANEL_END.y),
			BAR_STRIPE_COLOR
		);

		AddRectFilledScaled(
			ImVec2(PANEL_START.x + Menu::STRIPE2_START, PANEL_START.y),
			ImVec2(PANEL_START.x + Menu::STRIPE2_END,   PANEL_END.y),
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


	static void drawAnimatedSelectable(const GuiContext&, Menu&, const Selectable&, bool isSelected, const ImVec2& region, GLuint pointerTextureId);
	static bool drawPlainSelectable(const GuiContext&, Menu&, const Selectable&, bool isSelected);
	static void drawSquare(const GuiContext&, float textHeight, const ImVec4& textColor);


	bool MenuSelect::Selectable::draw(const GuiContext& context, Menu& menu, const ImVec2& region, GLuint pointerTextureId, bool isSelected) {
		if (isSelected) {
			time = min(time + context.getDeltaTime(), SELECT_ANIMATION_TIME);
		} else {
			time = clamp(time - context.getDeltaTime(), 0.0f, SELECT_FADE_TIME);
		}

		if (time > 0) {
			drawAnimatedSelectable(context, menu, *this, isSelected, region, pointerTextureId);
			return false;
		}

		return drawPlainSelectable(context, menu, *this, isSelected);
	}


	static void drawAnimatedSelectable(const GuiContext& context, Menu& menu, const Selectable& selectable, bool isSelected, const ImVec2& region, GLuint pointerTextureId) {
		const float progress = selectable.getTime() / SELECT_ANIMATION_TIME;
		const float fadeProgress = clamp(selectable.getTime() / SELECT_FADE_TIME, 0.0f, 1.0f);

		const ImVec4 stripeColor = alpha(ImGui::ColorConvertU32ToFloat4(Menu::SELECTION_COLOR), fadeProgress);
		const ImVec4 textColor = mix(ImGui::GetStyleColorVec4(ImGuiCol_Text), ImGui::ColorConvertU32ToFloat4(Menu::SELECTION_TEXT_COLOR), fadeProgress);

		const float rectX = isSelected ?
				(1.0f - exp(-6.0f * progress)) * (1.0f / (1.0f - exp(-6.0f))) : fadeProgress;

		const ImVec2 padding = context.scaleY(0, STRIPE_HEIGHT + SELECTABLE_MARGIN_Y);
		const ImVec2 stripeSize = context.scaleY(region.x, STRIPE_HEIGHT);
		const float textHeight = ImGui::CalcTextSize(selectable.getLabel().c_str(), nullptr, true).y;
		const ImVec2 rectSize = {
				rectX * (ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x),
				textHeight + context.scaleY(SELECTABLE_PADDING_XY * 2)
		};

		const float top = ImGui::GetCursorPosY();

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

		const ImVec2 pos = ImGui::GetCursorPos();
		ImGui::SetCursorPosX(pos.x - context.scaleX(BAR_SIZE.x - POINTER_TEXTURE_OFFSET_X));
		ImGui::Image(pointerTextureId, context.scaleVec(POINTER_TEXTURE_SIZE), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, fadeProgress), TRANSPARENT);
		ImGui::SetCursorPos(pos);

		ImGui::Dummy(context.scaleY(0, SELECTABLE_PADDING_XY));

		ImGui::PushStyleColor(ImGuiCol_Text, textColor);
		const bool clicked = ImGui::Selectable(selectable.getLabel().c_str(), true);
		ImGui::PopStyleColor();

		if (clicked || ImGui::IsKeyPressed(ImGuiKey_Enter)) {
			menu.loadLevel(selectable.getPath());
		}

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


	static bool drawPlainSelectable(const GuiContext& context, Menu& menu, const Selectable& selectable, bool isSelected) {
		const ImVec2 padding = context.scaleY(0, STRIPE_HEIGHT + SELECTABLE_MARGIN_Y + SELECTABLE_PADDING_XY);

		ImGui::Dummy(padding);

		const bool clicked = ImGui::Selectable(selectable.getLabel().c_str(), false);

		if (clicked) {
			menu.loadLevel(selectable.getPath());
		}

		if (clicked || ImGui::IsItemHovered()) {
			isSelected = true;
		}

		drawSquare(context, ImGui::CalcTextSize(selectable.getLabel().c_str(), nullptr, true).y, ImGui::GetStyleColorVec4(ImGuiCol_Text));

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