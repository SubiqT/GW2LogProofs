#ifndef IMGUIEXTENSIONS_H
#define IMGUIEXTENSIONS_H

#include "imgui/imgui.h"

namespace ImGui {
	static void AddUnderLine(ImColor col_) {
		ImVec2 min = ImGui::GetItemRectMin();
		ImVec2 max = ImGui::GetItemRectMax();
		min.y = max.y;
		ImGui::GetWindowDrawList()->AddLine(min, max, col_, 1.0f);
	}

	bool TextURL(const char* name_, bool SameLineBefore_ = false, bool SameLineAfter_ = false) {
		bool clicked = false;

		if (true == SameLineBefore_) { ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x); }
		ImGui::Text(name_);
		if (ImGui::IsItemHovered()) {
			if (ImGui::IsMouseClicked(0)) {
				clicked = true;
			}
			AddUnderLine(ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered]);
		} else {
			AddUnderLine(ImGui::GetStyle().Colors[ImGuiCol_Button]);
		}
		if (true == SameLineAfter_) { ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x); }

		return clicked;
	}
} // namespace ImGui

#endif
