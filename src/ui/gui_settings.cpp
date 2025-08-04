#include "../core/settings.h"
#include "../core/shared.h"
#include "../core/tab_config.h"
#include "../imgui/imgui.h"
#include <algorithm>
#include <unordered_map>

void ToggleShowWindowLogProofs(const char* keybindIdentifier, bool isRelease) {
	if (isRelease)
		return;
	Settings::ShowWindowLogProofs = !Settings::ShowWindowLogProofs;
	Settings::Settings[WINDOW_LOG_PROOFS_KEY][SHOW_WINDOW_LOG_PROOFS] = Settings::ShowWindowLogProofs;
	Settings::Save(SettingsPath);
}

void RegisterQuickAccessShortcut() {
	APIDefs->Log(ELogLevel_DEBUG, ADDON_NAME, "Registering Log Proofs quick access shortcut");
	APIDefs->QuickAccess.Add("SHORTCUT_LOG_PROOFS", "TEX_LOG_NORMAL", "TEX_LOG_HOVER", KB_TOGGLE_SHOW_WINDOW_LOG_PROOFS, "Toggle Log Proofs Window");
}

void DeregisterQuickAccessShortcut() {
	APIDefs->Log(ELogLevel_DEBUG, ADDON_NAME, "Deregistering Log Proofs quick access shortcut");
	APIDefs->QuickAccess.Remove("SHORTCUT_LOG_PROOFS");
}

static void DrawWindowSizingOptions() {
	if (ImGui::BeginTable("WindowSizingTable", 2, ImGuiTableFlags_SizingStretchProp)) {
		ImGui::TableSetupColumn("Setting", ImGuiTableColumnFlags_WidthFixed, 150.0f);
		ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);

		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text("Auto Resize");
		ImGui::TableNextColumn();
		if (ImGui::Checkbox("##AutoResize", &Settings::WindowAutoResize)) {
			Settings::Settings[WINDOW_LOG_PROOFS_KEY][WINDOW_AUTO_RESIZE] = Settings::WindowAutoResize;
			Settings::Save(SettingsPath);
		}

		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text("Restrict Size");
		ImGui::TableNextColumn();
		if (ImGui::Checkbox("##RestrictSize", &Settings::WindowRestrictSize)) {
			Settings::Settings[WINDOW_LOG_PROOFS_KEY][WINDOW_RESTRICT_SIZE] = Settings::WindowRestrictSize;
			Settings::Save(SettingsPath);
		}

		ImGui::EndTable();
	}

	if (Settings::WindowRestrictSize) {
		if (ImGui::BeginTable("WindowSizeConstraintsTable", 3, ImGuiTableFlags_SizingStretchProp)) {
			ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, 60.0f);
			ImGui::TableSetupColumn("Min", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableSetupColumn("Max", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableHeadersRow();

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("Width");
			ImGui::TableNextColumn();
			if (ImGui::SliderFloat("##MinWidth", &Settings::MinWindowWidth, 100.0f, 1500.0f, "%.0f px")) {
				Settings::MinWindowWidth = std::clamp(Settings::MinWindowWidth, 100.0f, 1500.0f);
				if (Settings::MinWindowWidth > Settings::MaxWindowWidth) {
					Settings::MaxWindowWidth = Settings::MinWindowWidth;
					Settings::Settings[WINDOW_LOG_PROOFS_KEY][MAX_WINDOW_WIDTH] = Settings::MaxWindowWidth;
				}
				Settings::Settings[WINDOW_LOG_PROOFS_KEY][MIN_WINDOW_WIDTH] = Settings::MinWindowWidth;
				Settings::Save(SettingsPath);
			}
			ImGui::TableNextColumn();
			if (ImGui::SliderFloat("##MaxWidth", &Settings::MaxWindowWidth, 100.0f, 1500.0f, "%.0f px")) {
				Settings::MaxWindowWidth = std::clamp(Settings::MaxWindowWidth, 100.0f, 1500.0f);
				if (Settings::MaxWindowWidth < Settings::MinWindowWidth) {
					Settings::MinWindowWidth = Settings::MaxWindowWidth;
					Settings::Settings[WINDOW_LOG_PROOFS_KEY][MIN_WINDOW_WIDTH] = Settings::MinWindowWidth;
				}
				Settings::Settings[WINDOW_LOG_PROOFS_KEY][MAX_WINDOW_WIDTH] = Settings::MaxWindowWidth;
				Settings::Save(SettingsPath);
			}

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("Height");
			ImGui::TableNextColumn();
			if (ImGui::SliderFloat("##MinHeight", &Settings::MinWindowHeight, 100.0f, 1500.0f, "%.0f px")) {
				Settings::MinWindowHeight = std::clamp(Settings::MinWindowHeight, 100.0f, 1500.0f);
				if (Settings::MinWindowHeight > Settings::MaxWindowHeight) {
					Settings::MaxWindowHeight = Settings::MinWindowHeight;
					Settings::Settings[WINDOW_LOG_PROOFS_KEY][MAX_WINDOW_HEIGHT] = Settings::MaxWindowHeight;
				}
				Settings::Settings[WINDOW_LOG_PROOFS_KEY][MIN_WINDOW_HEIGHT] = Settings::MinWindowHeight;
				Settings::Save(SettingsPath);
			}
			ImGui::TableNextColumn();
			if (ImGui::SliderFloat("##MaxHeight", &Settings::MaxWindowHeight, 100.0f, 1500.0f, "%.0f px")) {
				Settings::MaxWindowHeight = std::clamp(Settings::MaxWindowHeight, 100.0f, 1500.0f);
				if (Settings::MaxWindowHeight < Settings::MinWindowHeight) {
					Settings::MinWindowHeight = Settings::MaxWindowHeight;
					Settings::Settings[WINDOW_LOG_PROOFS_KEY][MIN_WINDOW_HEIGHT] = Settings::MinWindowHeight;
				}
				Settings::Settings[WINDOW_LOG_PROOFS_KEY][MAX_WINDOW_HEIGHT] = Settings::MaxWindowHeight;
				Settings::Save(SettingsPath);
			}

			ImGui::EndTable();
		}
	}
}

static void DrawColumnOptions() {
	if (ImGui::BeginTable("ColumnSizingTable", 2, ImGuiTableFlags_SizingStretchProp)) {
		ImGui::TableSetupColumn("Setting", ImGuiTableColumnFlags_WidthFixed, 120.0f);
		ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);

		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text("Account Width");
		ImGui::TableNextColumn();
		if (ImGui::SliderFloat("##AccountSize", &Settings::ColumnSizeAccount, 40.0f, 400.0f, "%.0f px")) {
			Settings::ColumnSizeAccount = std::clamp(Settings::ColumnSizeAccount, 40.0f, 400.0f);
			Settings::Settings[WINDOW_LOG_PROOFS_KEY][COLUMN_ACCOUNT_SIZE] = Settings::ColumnSizeAccount;
			Settings::Save(SettingsPath);
		}

		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text("Id Width");
		ImGui::TableNextColumn();
		if (ImGui::SliderFloat("##KpmeIdSize", &Settings::ColumnSizeKpmeId, 8.0f, 128.0f, "%.0f px")) {
			Settings::ColumnSizeKpmeId = std::clamp(Settings::ColumnSizeKpmeId, 8.0f, 128.0f);
			Settings::Settings[WINDOW_LOG_PROOFS_KEY][COLUMN_KPME_ID_SIZE] = Settings::ColumnSizeKpmeId;
			Settings::Save(SettingsPath);
		}

		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text("Proof Width");
		ImGui::TableNextColumn();
		if (ImGui::SliderFloat("##BossesSize", &Settings::ColumnSizeBosses, 8.0f, 128.0f, "%.0f px")) {
			Settings::ColumnSizeBosses = std::clamp(Settings::ColumnSizeBosses, 8.0f, 128.0f);
			Settings::Settings[WINDOW_LOG_PROOFS_KEY][COLUMN_BOSSES_SIZE] = Settings::ColumnSizeBosses;
			Settings::Save(SettingsPath);
		}

		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text("Proof Icon Size");
		ImGui::TableNextColumn();
		if (ImGui::SliderFloat("##BossIconScale", &Settings::BossIconScale, 8.0f, 128.0f, "%.0f px")) {
			Settings::BossIconScale = std::clamp(Settings::BossIconScale, 8.0f, 128.0f);
			Settings::Settings[WINDOW_LOG_PROOFS_KEY][BOSS_ICON_SCALE] = Settings::BossIconScale;
			Settings::Save(SettingsPath);
		}

		ImGui::EndTable();
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

	// Group proofs by default tab structure
	std::unordered_map<std::string, std::vector<ProofOption>> tabGroups;
	for (const auto& proof : availableProofs) {
		std::string tabName;
		if (proof.category == "Raid") {
			if (proof.bossType == "Normal") tabName = "Raids";
			else if (proof.bossType == "CM")
				tabName = "Raid CMs";
			else if (proof.bossType == "LCM")
				tabName = "Raid LMs";
		} else if (proof.category == "Strike") {
			if (proof.bossType == "Normal") tabName = "Strikes";
			else if (proof.bossType == "CM")
				tabName = "Strike CMs";
			else if (proof.bossType == "LCM")
				tabName = "Strike LMs";
		} else if (proof.category == "Fractal") {
			if (proof.bossType == "CM") tabName = "Fractal CMs";
			else if (proof.bossType == "LCM")
				tabName = "Fractal LMs";
		} else {
			// For other categories, use category + boss type
			tabName = proof.category;
			if (proof.bossType == "CM") tabName += " CM";
			else if (proof.bossType == "LCM")
				tabName += " LCM";
		}
		tabGroups[tabName].push_back(proof);
	}

	// Define tab order to match default tab config
	std::vector<std::string> tabOrder = {
			"Raids", "Raid CMs", "Raid LMs",
			"Strikes", "Strike CMs", "Strike LMs",
			"Fractal CMs", "Fractal LMs"
	};

	// Draw tabs in order
	for (const auto& tabName : tabOrder) {
		if (tabGroups.find(tabName) != tabGroups.end()) {
			if (ImGui::CollapsingHeader(tabName.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
				for (const auto& proof : tabGroups[tabName]) {
					bool selected = std::find_if(selectedProofs.begin(), selectedProofs.end(), [&proof](const ProofSelection& sel) {
										return sel.proofId == proof.id && sel.bossType == proof.bossType;
									})
									!= selectedProofs.end();
					if (ImGui::Checkbox(proof.displayName.c_str(), &selected)) {
						if (selected) {
							selectedProofs.push_back({proof.id, proof.bossType});
						} else {
							selectedProofs.erase(std::remove_if(selectedProofs.begin(), selectedProofs.end(), [&proof](const ProofSelection& sel) {
													 return sel.proofId == proof.id && sel.bossType == proof.bossType;
												 }),
												 selectedProofs.end());
						}
					}
				}
			}
		}
	}

	// Handle remaining tabs not in the predefined order
	for (const auto& [tabName, proofs] : tabGroups) {
		if (std::find(tabOrder.begin(), tabOrder.end(), tabName) == tabOrder.end()) {
			if (ImGui::CollapsingHeader(tabName.c_str())) {
				for (const auto& proof : proofs) {
					bool selected = std::find_if(selectedProofs.begin(), selectedProofs.end(), [&proof](const ProofSelection& sel) {
										return sel.proofId == proof.id && sel.bossType == proof.bossType;
									})
									!= selectedProofs.end();
					if (ImGui::Checkbox(proof.displayName.c_str(), &selected)) {
						if (selected) {
							selectedProofs.push_back({proof.id, proof.bossType});
						} else {
							selectedProofs.erase(std::remove_if(selectedProofs.begin(), selectedProofs.end(), [&proof](const ProofSelection& sel) {
													 return sel.proofId == proof.id && sel.bossType == proof.bossType;
												 }),
												 selectedProofs.end());
						}
					}
				}
			}
		}
	}
}

void DrawTabEditor(const std::string& providerId, CustomTab& tab) {
	ImGui::PushID(tab.id.c_str());

	if (ImGui::BeginTable(("TabEditor_" + tab.id).c_str(), 2, ImGuiTableFlags_SizingStretchProp)) {
		ImGui::TableSetupColumn("Setting", ImGuiTableColumnFlags_WidthFixed, 120.0f);
		ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);

		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text("Name");
		ImGui::TableNextColumn();
		char nameBuffer[256];
		strcpy_s(nameBuffer, tab.displayName.c_str());
		if (ImGui::InputText("##Name", nameBuffer, sizeof(nameBuffer))) {
			tab.displayName = nameBuffer;
			TabConfigManager::Instance().UpdateTab(providerId, tab.id, tab);
		}

		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text("Visible");
		ImGui::TableNextColumn();
		if (ImGui::Checkbox("##Visible", &tab.visible)) {
			TabConfigManager::Instance().UpdateTab(providerId, tab.id, tab);
		}

		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text("Actions");
		ImGui::TableNextColumn();
		if (ImGui::Button("Delete Tab")) {
			TabConfigManager::Instance().DeleteTab(providerId, tab.id);
		}

		ImGui::EndTable();
	}

	if (ImGui::CollapsingHeader("Proof Selection")) {
		auto oldProofs = tab.proofs;
		DrawProofSelector(providerId, tab.proofs);
		if (oldProofs != tab.proofs) {
			TabConfigManager::Instance().UpdateTab(providerId, tab.id, tab);
		}
	}

	ImGui::PopID();
	ImGui::Separator();
}

void DrawCustomTabEditor(const std::string& providerId) {
	auto config = TabConfigManager::Instance().GetProviderConfig(providerId);

	if (ImGui::BeginTable(("CustomTabList_" + providerId).c_str(), 4, ImGuiTableFlags_Borders | ImGuiTableFlags_SizingStretchProp)) {
		ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableSetupColumn("Visible", ImGuiTableColumnFlags_WidthFixed, 60.0f);
		ImGui::TableSetupColumn("Proofs", ImGuiTableColumnFlags_WidthFixed, 60.0f);
		ImGui::TableSetupColumn("Actions", ImGuiTableColumnFlags_WidthFixed, 80.0f);
		ImGui::TableHeadersRow();

		for (auto& tab : config.tabs) {
			ImGui::PushID(tab.id.c_str());
			ImGui::TableNextRow();

			ImGui::TableNextColumn();
			char nameBuffer[256];
			strcpy_s(nameBuffer, tab.displayName.c_str());
			ImGui::SetNextItemWidth(-1);
			if (ImGui::InputText("##Name", nameBuffer, sizeof(nameBuffer))) {
				tab.displayName = nameBuffer;
				TabConfigManager::Instance().UpdateTab(providerId, tab.id, tab);
			}

			ImGui::TableNextColumn();
			if (ImGui::Checkbox("##Visible", &tab.visible)) {
				TabConfigManager::Instance().UpdateTab(providerId, tab.id, tab);
			}

			ImGui::TableNextColumn();
			ImGui::Text("%d", static_cast<int>(tab.proofs.size()));

			ImGui::TableNextColumn();
			if (ImGui::SmallButton("Edit")) {
				ImGui::OpenPopup(("ProofSelector_" + tab.id).c_str());
			}
			ImGui::SameLine();
			if (ImGui::SmallButton("Delete")) {
				TabConfigManager::Instance().DeleteTab(providerId, tab.id);
			}

			ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
			if (ImGui::BeginPopupModal(("ProofSelector_" + tab.id).c_str(), nullptr, 0)) {
				ImGui::Text("Select proofs for: %s", tab.displayName.c_str());
				ImGui::Separator();

				if (ImGui::BeginChild("ProofList", ImVec2(0, -30), true)) {
					auto oldProofs = tab.proofs;
					DrawProofSelector(providerId, tab.proofs);
					if (oldProofs != tab.proofs) {
						TabConfigManager::Instance().UpdateTab(providerId, tab.id, tab);
					}
				}
				ImGui::EndChild();

				ImGui::Separator();
				if (ImGui::Button("Close")) {
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}

			ImGui::PopID();
		}

		ImGui::EndTable();
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

static void DrawGeneralSettings() {
	if (ImGui::BeginTable("GeneralSettingsTable", 2, ImGuiTableFlags_SizingStretchProp)) {
		ImGui::TableSetupColumn("Setting", ImGuiTableColumnFlags_WidthFixed, 200.0f);
		ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);

		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text("Show Quick Access Shortcut");
		ImGui::TableNextColumn();
		if (ImGui::Checkbox("##ShowQuickAccess", &Settings::ShowQuickAccessShortcut)) {
			Settings::Settings[SHOW_QUICK_ACCESS_SHORTCUT] = Settings::ShowQuickAccessShortcut;
			Settings::Save(SettingsPath);
			if (Settings::ShowQuickAccessShortcut)
				RegisterQuickAccessShortcut();
			else
				DeregisterQuickAccessShortcut();
		}

		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text("Visibility Mode");
		ImGui::TableNextColumn();
		const char* visibilityModes[] = {"Show all accounts", "Hide accounts with no data"};
		int currentMode = Settings::IncludeMissingAccounts ? 0 : 1;
		if (ImGui::BeginCombo("##VisibilityMode", visibilityModes[currentMode])) {
			for (int i = 0; i < 2; i++) {
				bool is_selected = (currentMode == i);
				if (ImGui::Selectable(visibilityModes[i], is_selected)) {
					Settings::IncludeMissingAccounts = (i == 0);
					Settings::Settings[WINDOW_LOG_PROOFS_KEY][INCLUDE_MISSING_ACCOUNTS] = Settings::IncludeMissingAccounts;
					Settings::Save(SettingsPath);
				}
				if (is_selected) ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text("Linked Accounts Mode");
		ImGui::TableNextColumn();
		const char* modeNames[] = {"Hide linked accounts", "Combine linked accounts", "Split linked accounts"};
		if (ImGui::BeginCombo("##LinkedMode", modeNames[Settings::LinkedAccountsMode])) {
			for (int i = 0; i < 3; i++) {
				bool is_selected = (Settings::LinkedAccountsMode == i);
				if (ImGui::Selectable(modeNames[i], is_selected)) {
					Settings::LinkedAccountsMode = static_cast<LinkedAccountMode>(i);
					Settings::Settings[WINDOW_LOG_PROOFS_KEY][LINKED_ACCOUNTS_MODE] = Settings::LinkedAccountsMode;
					Settings::Save(SettingsPath);
				}
				if (is_selected) ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		ImGui::EndTable();
	}
}

static void DrawNetworkSettings() {
	if (ImGui::BeginTable("NetworkSettingsTable", 2, ImGuiTableFlags_SizingStretchProp)) {
		ImGui::TableSetupColumn("Setting", ImGuiTableColumnFlags_WidthFixed, 200.0f);
		ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);

		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text("Cache Timeout (minutes)");
		ImGui::TableNextColumn();
		if (ImGui::SliderInt("##CacheTimeout", &Settings::CacheTimeoutMinutes, 1, 30)) {
			Settings::CacheTimeoutMinutes = std::clamp(Settings::CacheTimeoutMinutes, 1, 30);
			Settings::Settings[WINDOW_LOG_PROOFS_KEY][CACHE_TIMEOUT_MINUTES] = Settings::CacheTimeoutMinutes;
			Settings::Save(SettingsPath);
		}

		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text("Max Retry Attempts");
		ImGui::TableNextColumn();
		if (ImGui::SliderInt("##MaxRetries", &Settings::MaxRetryAttempts, 1, 10)) {
			Settings::MaxRetryAttempts = std::clamp(Settings::MaxRetryAttempts, 1, 10);
			Settings::Settings[WINDOW_LOG_PROOFS_KEY][MAX_RETRY_ATTEMPTS] = Settings::MaxRetryAttempts;
			Settings::Save(SettingsPath);
		}

		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text("Max Concurrent Requests");
		ImGui::TableNextColumn();
		static bool showConcurrencyWarning = false;
		if (ImGui::SliderInt("##MaxConcurrentRequests", &Settings::MaxConcurrentRequests, 1, 10)) {
			Settings::MaxConcurrentRequests = std::clamp(Settings::MaxConcurrentRequests, 1, 10);
			Settings::Settings[WINDOW_LOG_PROOFS_KEY][MAX_CONCURRENT_REQUESTS] = Settings::MaxConcurrentRequests;
			Settings::Save(SettingsPath);
			showConcurrencyWarning = true;
		}
		if (showConcurrencyWarning) {
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::TableNextColumn();
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.8f, 0.0f, 1.0f));
			ImGui::TextWrapped("Reload the addon for thread pool changes to take effect.");
			ImGui::PopStyleColor();
		}

		ImGui::EndTable();
	}
}

void RenderWindowSettings() {
	ImGui::Indent(10.0f);
	if (ImGui::CollapsingHeader("General")) {
		DrawGeneralSettings();
	}

	if (ImGui::CollapsingHeader("Network Settings")) {
		DrawNetworkSettings();
	}

	if (ImGui::CollapsingHeader("Window Sizing")) {
		DrawWindowSizingOptions();
	}

	if (ImGui::CollapsingHeader("Column Sizing")) {
		DrawColumnOptions();
	}

	if (ImGui::CollapsingHeader("Hover Settings")) {
		DrawHoverOptions();
	}

	if (ImGui::CollapsingHeader("Tab Configuration")) {
		DrawTabConfigurationPanel();
	}
	ImGui::Unindent(10.0f);
}