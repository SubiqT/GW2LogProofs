#include "imgui/imgui.h"
#include "settings.h"
#include "shared.h"

void ToggleShowWindowLogProofs(const char* keybindIdentifier, bool isRelease) {
	if (isRelease)
		return;
	APIDefs->Log(ELogLevel_DEBUG, ADDON_NAME, std::format("Keybind {} was pressed.", keybindIdentifier).c_str());
	Settings::ShowWindowLogProofs = !Settings::ShowWindowLogProofs;
	Settings::Settings[WINDOW_LOG_PROOFS_KEY][SHOW_WINDOW_LOG_PROOFS] = Settings::ShowWindowLogProofs;
	Settings::Save(SettingsPath);
}

void RegisterQuickAccessShortcut() {
	APIDefs->Log(ELogLevel_DEBUG, ADDON_NAME, "registering quick access shortcut");
	APIDefs->QuickAccess.Add("SHORTCUT_LOG_PROOFS", "TEX_LOG_NORMAL", "TEX_LOG_HOVER", KB_TOGGLE_SHOW_WINDOW_LOG_PROOFS, "Toggle Log Proofs Window");
}

void DeregisterQuickAccessShortcut() {
	APIDefs->Log(ELogLevel_DEBUG, ADDON_NAME, "deregistering quick access shortcut");
	APIDefs->QuickAccess.Remove("SHORTCUT_LOG_PROOFS");
}

void DrawWindowSizingOptions() {
	ImGui::Text("Window Sizing");
	if (ImGui::SliderFloat("Min Width", &Settings::MinWindowWidth, 100.0f, 1500.0f, "%.3f px")) {
		Settings::Settings[WINDOW_LOG_PROOFS_KEY][MIN_WINDOW_WIDTH] = Settings::MinWindowWidth;
		Settings::Save(SettingsPath);
	}
	if (ImGui::SliderFloat("Max Width", &Settings::MaxWindowWidth, 100.0f, 1500.0f, "%.3f px")) {
		Settings::Settings[WINDOW_LOG_PROOFS_KEY][MAX_WINDOW_WIDTH] = Settings::MaxWindowWidth;
		Settings::Save(SettingsPath);
	}
	if (ImGui::SliderFloat("Min Height", &Settings::MinWindowHeight, 100.0f, 1500.0f, "%.3f px")) {
		Settings::Settings[WINDOW_LOG_PROOFS_KEY][MIN_WINDOW_HEIGHT] = Settings::MinWindowHeight;
		Settings::Save(SettingsPath);
	}
	if (ImGui::SliderFloat("Max Height", &Settings::MaxWindowHeight, 100.0f, 1500.0f, "%.3f px")) {
		Settings::Settings[WINDOW_LOG_PROOFS_KEY][MAX_WINDOW_HEIGHT] = Settings::MaxWindowHeight;
		Settings::Save(SettingsPath);
	}
}

void DrawTabsOptions() {
	ImGui::Text("Wingman Tabs");
	if (ImGui::Checkbox("Raids", &Settings::ShowTabRaidsNormal)) {
		Settings::Settings[WINDOW_LOG_PROOFS_KEY][SHOW_TAB_RAIDS_NORMAL] = Settings::ShowTabRaidsNormal;
		Settings::Save(SettingsPath);
	}
	if (ImGui::Checkbox("Raid CMs", &Settings::ShowTabRaidsCM)) {
		Settings::Settings[WINDOW_LOG_PROOFS_KEY][SHOW_TAB_RAIDS_CM] = Settings::ShowTabRaidsCM;
		Settings::Save(SettingsPath);
	}
	if (ImGui::Checkbox("Raid LMs", &Settings::ShowTabRaidsLM)) {
		Settings::Settings[WINDOW_LOG_PROOFS_KEY][SHOW_TAB_RAIDS_LM] = Settings::ShowTabRaidsLM;
		Settings::Save(SettingsPath);
	}
	if (ImGui::Checkbox("Fractal CMs", &Settings::ShowTabFractalsCM)) {
		Settings::Settings[WINDOW_LOG_PROOFS_KEY][SHOW_TAB_FRACTALS_CM] = Settings::ShowTabFractalsCM;
		Settings::Save(SettingsPath);
	}
	if (ImGui::Checkbox("Strikes", &Settings::ShowTabStrikesNormal)) {
		Settings::Settings[WINDOW_LOG_PROOFS_KEY][SHOW_TAB_STRIKES_NORMAL] = Settings::ShowTabStrikesNormal;
		Settings::Save(SettingsPath);
	}
	if (ImGui::Checkbox("Strike CMs", &Settings::ShowTabStrikesCM)) {
		Settings::Settings[WINDOW_LOG_PROOFS_KEY][SHOW_TAB_STRIKES_CM] = Settings::ShowTabStrikesCM;
		Settings::Save(SettingsPath);
	}
	if (ImGui::Checkbox("Strike LMs", &Settings::ShowTabStrikesLM)) {
		Settings::Settings[WINDOW_LOG_PROOFS_KEY][SHOW_TAB_STRIKES_LM] = Settings::ShowTabStrikesLM;
		Settings::Save(SettingsPath);
	}
	ImGui::Text("KPME Tabs");
	if (ImGui::Checkbox("Summary", &Settings::ShowTabKpmeSummary)) {
		Settings::Settings[WINDOW_LOG_PROOFS_KEY][SHOW_TAB_KPME_SUMMARY] = Settings::ShowTabKpmeSummary;
		Settings::Save(SettingsPath);
	}
	if (ImGui::Checkbox("Raid Tokens", &Settings::ShowTabKpmeRaidTokens)) {
		Settings::Settings[WINDOW_LOG_PROOFS_KEY][SHOW_TAB_KPME_RAID_TOKENS] = Settings::ShowTabKpmeRaidTokens;
		Settings::Save(SettingsPath);
	}
	if (ImGui::Checkbox("Raid CM Coffers", &Settings::ShowTabKpmeRaidCMCoffers)) {
		Settings::Settings[WINDOW_LOG_PROOFS_KEY][SHOW_TAB_KPME_RAID_CM_COFFERS] = Settings::ShowTabKpmeRaidCMCoffers;
		Settings::Save(SettingsPath);
	}
	if (ImGui::Checkbox("Strike Coffers", &Settings::ShowTabKpmeStrikeCoffers)) {
		Settings::Settings[WINDOW_LOG_PROOFS_KEY][SHOW_TAB_KPME_STRIKE_COFFERS] = Settings::ShowTabKpmeStrikeCoffers;
		Settings::Save(SettingsPath);
	}
	if (ImGui::Checkbox("Strike CM Coffers", &Settings::ShowTabKpmeStrikeCMCoffers)) {
		Settings::Settings[WINDOW_LOG_PROOFS_KEY][SHOW_TAB_KPME_STRIKE_CM_COFFERS] = Settings::ShowTabKpmeStrikeCMCoffers;
		Settings::Save(SettingsPath);
	}
}

void DrawColumnOptions() {
	ImGui::Text("Columns");
	if (ImGui::SliderFloat("Account Size", &Settings::ColumnSizeAccount, 40.0f, 400.0f, "%.3f px")) {
		Settings::Settings[WINDOW_LOG_PROOFS_KEY][COLUMN_ACCOUNT_SIZE] = Settings::ColumnSizeAccount;
		Settings::Save(SettingsPath);
	}
	if (ImGui::SliderFloat("Bosses Size", &Settings::ColumnSizeBosses, 8.0f, 128.0f, "%.3f px")) {
		Settings::Settings[WINDOW_LOG_PROOFS_KEY][COLUMN_BOSSES_SIZE] = Settings::ColumnSizeBosses;
		Settings::Save(SettingsPath);
	}
	if (ImGui::SliderFloat("KPME Id Size", &Settings::ColumnSizeKpmeId, 8.0f, 128.0f, "%.3f px")) {
		Settings::Settings[WINDOW_LOG_PROOFS_KEY][COLUMN_KPME_ID_SIZE] = Settings::ColumnSizeKpmeId;
		Settings::Save(SettingsPath);
	}
}

void DrawHoverOptions() {
	ImGui::Text("Hover");
	if (ImGui::Checkbox("Enabled##HoverEnabled", &Settings::hoverEnabled)) {
		Settings::Settings[WINDOW_LOG_PROOFS_KEY][HOVER_ENABLED] = Settings::hoverEnabled;
		Settings::Save(SettingsPath);
	}
	if (ImGui::ColorPicker4("HoverColour##HoverColourPicker", (float*) &Settings::hoverColourBuffer)) {
		Settings::hoverColour = ImGui::ColorConvertFloat4ToU32(Settings::hoverColourBuffer);
		Settings::Settings[WINDOW_LOG_PROOFS_KEY][HOVER_COLOUR] = Settings::hoverColour;
		Settings::Save(SettingsPath);
	}
}

void RenderWindowSettings() {
	if (ImGui::Checkbox("Show Quick Access Shortcut", &Settings::ShowQuickAccessShortcut)) {
		Settings::Settings[SHOW_QUICK_ACCESS_SHORTCUT] = Settings::ShowQuickAccessShortcut;
		Settings::Save(SettingsPath);
		if (Settings::ShowQuickAccessShortcut)
			RegisterQuickAccessShortcut();
		else
			DeregisterQuickAccessShortcut();
	}
	if (ImGui::Checkbox("Show Window", &Settings::ShowWindowLogProofs)) {
		Settings::Settings[WINDOW_LOG_PROOFS_KEY][SHOW_WINDOW_LOG_PROOFS] = Settings::ShowWindowLogProofs;
		Settings::Save(SettingsPath);
	}
	DrawWindowSizingOptions();
	DrawColumnOptions();
	DrawTabsOptions();
	DrawHoverOptions();
}