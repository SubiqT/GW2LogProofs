#include "../core/settings.h"
#include "../core/shared.h"
#include "../core/tab_config.h"
#include "../imgui/imgui.h"
#include <algorithm>
#include <unordered_map>

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

static void DrawWindowSizingOptions() {
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

static void DrawColumnOptions() {
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

static void DrawHoverOptions() {
	static bool showColourModal = false;
	
	if (ImGui::Checkbox("Hover Enabled", &Settings::hoverEnabled)) {
		Settings::Settings[WINDOW_LOG_PROOFS_KEY][HOVER_ENABLED] = Settings::hoverEnabled;
		Settings::Save(SettingsPath);
	}
	
	if (Settings::hoverEnabled) {
		ImGui::SameLine();
		ImVec4 colour = ImGui::ColorConvertU32ToFloat4(Settings::hoverColour);
		if (ImGui::ColorButton("Hover Colour", colour)) {
			showColourModal = true;
		}
	}
	
	if (showColourModal) {
		ImGui::OpenPopup("Select Hover Colour");
		showColourModal = false;
	}
	
	if (ImGui::BeginPopupModal("Select Hover Colour", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
		if (ImGui::ColorPicker4("##HoverColourPicker", (float*) &Settings::hoverColourBuffer)) {
			Settings::hoverColour = ImGui::ColorConvertFloat4ToU32(Settings::hoverColourBuffer);
			Settings::Settings[WINDOW_LOG_PROOFS_KEY][HOVER_COLOUR] = Settings::hoverColour;
			Settings::Save(SettingsPath);
		}
		if (ImGui::Button("Close")) {
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}

void DrawProofSelector(const std::string& providerId, std::vector<ProofSelection>& selectedProofs) {
	auto availableProofs = TabConfigManager::Instance().GetAvailableProofs(providerId);

	std::unordered_map<std::string, std::vector<ProofOption>> categorized;
	for (const auto& proof : availableProofs) {
		categorized[proof.category].push_back(proof);
	}

	for (const auto& [category, proofs] : categorized) {
		if (ImGui::TreeNode(category.c_str())) {
			for (const auto& proof : proofs) {
				bool selected = std::find_if(selectedProofs.begin(), selectedProofs.end(), 
					[&proof](const ProofSelection& sel) { 
						return sel.proofId == proof.id && sel.bossType == proof.bossType; 
					}) != selectedProofs.end();
				if (ImGui::Checkbox(proof.displayName.c_str(), &selected)) {
					if (selected) {
						selectedProofs.push_back({proof.id, proof.bossType});
					} else {
						selectedProofs.erase(std::remove_if(selectedProofs.begin(), selectedProofs.end(),
							[&proof](const ProofSelection& sel) { 
								return sel.proofId == proof.id && sel.bossType == proof.bossType; 
							}), selectedProofs.end());
					}
				}
			}
			ImGui::TreePop();
		}
	}
}

void DrawTabEditor(const std::string& providerId, CustomTab& tab) {
	ImGui::PushID(tab.id.c_str());

	char nameBuffer[256];
	strcpy_s(nameBuffer, tab.displayName.c_str());
	if (ImGui::InputText("Name", nameBuffer, sizeof(nameBuffer))) {
		tab.displayName = nameBuffer;
		TabConfigManager::Instance().UpdateTab(providerId, tab.id, tab);
	}

	if (ImGui::Checkbox("Visible", &tab.visible)) {
		TabConfigManager::Instance().UpdateTab(providerId, tab.id, tab);
	}

	if (ImGui::CollapsingHeader("Proof Selection")) {
		auto oldProofs = tab.proofs;
		DrawProofSelector(providerId, tab.proofs);
		if (oldProofs != tab.proofs) {
			TabConfigManager::Instance().UpdateTab(providerId, tab.id, tab);
		}
	}

	if (ImGui::Button("Delete Tab")) {
		TabConfigManager::Instance().DeleteTab(providerId, tab.id);
	}

	ImGui::PopID();
	ImGui::Separator();
}

void DrawCustomTabEditor(const std::string& providerId) {
	auto config = TabConfigManager::Instance().GetProviderConfig(providerId);

	for (auto& tab : config.tabs) {
		DrawTabEditor(providerId, tab);
	}

	if (ImGui::Button("Add New Tab")) {
		static int tabCounter = -1;
		if (tabCounter == -1) {
			tabCounter = 0;
			for (const auto& tab : config.tabs) {
				if (tab.id.substr(0, 11) == "custom_tab_") {
					int num = std::stoi(tab.id.substr(11));
					tabCounter = (tabCounter > num) ? tabCounter : num;
				}
			}
		}
		CustomTab newTab;
		newTab.id = "custom_tab_" + std::to_string(++tabCounter);
		newTab.displayName = "New Tab";
		newTab.visible = true;
		newTab.order = static_cast<int>(config.tabs.size());
		TabConfigManager::Instance().CreateTab(providerId, newTab);
	}
}

void DrawProviderTabConfig(const std::string& providerId) {
	auto config = TabConfigManager::Instance().GetProviderConfig(providerId);

	bool useCustomTabs = config.useCustomTabs;
	if (ImGui::Checkbox("Use Custom Tabs", &useCustomTabs)) {
		config.useCustomTabs = useCustomTabs;
		TabConfigManager::Instance().SetProviderConfig(providerId, config);
		TabConfigManager::Instance().SaveAndPersist();
	}

	if (config.useCustomTabs) {
		DrawCustomTabEditor(providerId);
	} else {
		ImGui::Text("Using default tabs. Enable custom tabs to configure.");
	}
}

void DrawTabConfigurationPanel() {
	ImGui::Text("Tab Configuration");
	if (ImGui::BeginTabBar("ProviderTabConfig")) {
		for (const auto& providerId : {"Wingman", "KPME"}) {
			if (ImGui::BeginTabItem(providerId)) {
				DrawProviderTabConfig(providerId);
				ImGui::EndTabItem();
			}
		}
		ImGui::EndTabBar();
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

	DrawHoverOptions();
	DrawWindowSizingOptions();
	DrawColumnOptions();
	DrawTabConfigurationPanel();
}