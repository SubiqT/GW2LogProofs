#ifndef IMGUIEXTENSIONS_H
#define IMGUIEXTENSIONS_H

#include "../imgui/imgui.h"
#include <string>

namespace ImGui {
	inline void AddUnderLine(ImColor col_) {
		ImVec2 min = ImGui::GetItemRectMin();
		ImVec2 max = ImGui::GetItemRectMax();
		min.y = max.y;
		ImGui::GetWindowDrawList()->AddLine(min, max, col_, 1.0f);
	}

	inline bool TextURL(const char* name_, bool SameLineBefore_ = false, bool SameLineAfter_ = false) {
		bool clicked = false;

		if (true == SameLineBefore_) { ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x); }
		ImGui::TextUnformatted(name_);
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

	inline int ResizeStringCallback(ImGuiInputTextCallbackData* data) {
		if (data->EventFlag == ImGuiInputTextFlags_CallbackResize) {
			auto* text = static_cast<std::string*>(data->UserData);
			text->resize(data->BufTextLen);
			data->Buf = text->data();
		}
		return 0;
	}

	// InputText that edits a std::string directly, growing it as needed.
	inline bool InputString(const char* label, std::string& text, ImGuiInputTextFlags flags = 0, const char* hint = nullptr) {
		flags |= ImGuiInputTextFlags_CallbackResize;
		if (hint) return ImGui::InputTextWithHint(label, hint, text.data(), text.capacity() + 1, flags, ResizeStringCallback, &text);
		return ImGui::InputText(label, text.data(), text.capacity() + 1, flags, ResizeStringCallback, &text);
	}
} // namespace ImGui

#endif
