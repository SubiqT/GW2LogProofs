#include "settings.h"
#include "shared.h"
#include "tab_config.h"

#include <filesystem>
#include <format>
#include <fstream>

#include "../imgui/imgui.h"

const char* SHOW_QUICK_ACCESS_SHORTCUT = "ShowQuickAccessShortcut";
const char* WINDOW_LOG_PROOFS_KEY = "WindowLogProofs";
const char* SHOW_WINDOW_LOG_PROOFS = "ShowWindow";

const char* MIN_WINDOW_WIDTH = "MinWindowWidth";
const char* MIN_WINDOW_HEIGHT = "MinWindowHeight";
const char* MAX_WINDOW_WIDTH = "MaxWindowWidth";
const char* MAX_WINDOW_HEIGHT = "MaxWindowHeight";

const char* SHOW_TAB_RAIDS_NORMAL = "ShowTabRaidsNormal";
const char* SHOW_TAB_RAIDS_CM = "ShowTabRaidsCM";
const char* SHOW_TAB_RAIDS_LM = "ShowTabRaidsLCM";
const char* SHOW_TAB_FRACTALS_CM = "ShowTabFractalsCM";
const char* SHOW_TAB_STRIKES_NORMAL = "ShowTabStrikesNormal";
const char* SHOW_TAB_STRIKES_CM = "ShowTabStrikesCM";
const char* SHOW_TAB_STRIKES_LM = "ShowTabStrikesLM";

const char* SHOW_TAB_KPME_SUMMARY = "ShowTabKpmeSummary";
const char* SHOW_TAB_KPME_RAID_TOKENS = "ShowTabKpmeRaidTokens";
const char* SHOW_TAB_KPME_RAID_CM_COFFERS = "ShowTabKpmeRaidCMCoffers";
const char* SHOW_TAB_KPME_STRIKE_COFFERS = "ShowTabKpmeStrikeCoffers";
const char* SHOW_TAB_KPME_STRIKE_CM_COFFERS = "ShowTabKpmeStrikeCMCoffers";

const char* COLUMN_ACCOUNT_SIZE = "ColumnAccountSize";
const char* COLUMN_BOSSES_SIZE = "ColumnBossesSize";
const char* COLUMN_KPME_ID_SIZE = "ColumnKpmeIdSize";

const char* SELECTED_DATA_SOURCE = "SelectedDataSource";
const char* INCLUDE_LINKED_ACCOUNTS = "IncludeLinkedAccounts";
const char* INCLUDE_MISSING_ACCOUNTS = "IncludeMissingAccounts";

const char* HOVER_ENABLED = "HoverEnabled";
const char* HOVER_COLOUR = "HoverColour";

const char* CUSTOM_TABS_ENABLED = "CustomTabsEnabled";
const char* PROVIDER_CONFIGS = "ProviderConfigs";
const char* MIGRATION_COMPLETED = "MigrationCompleted";

const char* LAZY_LOADING_ENABLED = "LazyLoadingEnabled";
const char* CACHE_TIMEOUT_MINUTES = "CacheTimeoutMinutes";
const char* MAX_RETRY_ATTEMPTS = "MaxRetryAttempts";

namespace Settings {
	std::mutex Mutex;
	json Settings = json::object();

	void Load(std::filesystem::path filePath) {
		if (!std::filesystem::exists(filePath))
			return;

		try {
			std::scoped_lock lck(Mutex);
			std::ifstream file(filePath);
			Settings = json::parse(file);
			file.close();
		} catch (json::parse_error& ex) {
			APIDefs->Log(ELogLevel_CRITICAL, ADDON_NAME, std::format("failed to parse settings.json file. exception details: {}", ex.what()).c_str());
		}

		if (!Settings[SHOW_QUICK_ACCESS_SHORTCUT].is_null()) {
			Settings[SHOW_QUICK_ACCESS_SHORTCUT].get_to<bool>(ShowQuickAccessShortcut);
		}

		if (!Settings[WINDOW_LOG_PROOFS_KEY][SHOW_WINDOW_LOG_PROOFS].is_null()) {
			Settings[WINDOW_LOG_PROOFS_KEY][SHOW_WINDOW_LOG_PROOFS].get_to<bool>(ShowWindowLogProofs);
		}

		/* Window Sizing */
		if (!Settings[WINDOW_LOG_PROOFS_KEY][MIN_WINDOW_WIDTH].is_null()) {
			Settings[WINDOW_LOG_PROOFS_KEY][MIN_WINDOW_WIDTH].get_to<float>(MinWindowWidth);
		}
		if (!Settings[WINDOW_LOG_PROOFS_KEY][MIN_WINDOW_HEIGHT].is_null()) {
			Settings[WINDOW_LOG_PROOFS_KEY][MIN_WINDOW_HEIGHT].get_to<float>(MinWindowHeight);
		}
		if (!Settings[WINDOW_LOG_PROOFS_KEY][MAX_WINDOW_WIDTH].is_null()) {
			Settings[WINDOW_LOG_PROOFS_KEY][MAX_WINDOW_WIDTH].get_to<float>(MaxWindowWidth);
		}
		if (!Settings[WINDOW_LOG_PROOFS_KEY][MAX_WINDOW_HEIGHT].is_null()) {
			Settings[WINDOW_LOG_PROOFS_KEY][MAX_WINDOW_HEIGHT].get_to<float>(MaxWindowHeight);
		}

		/* Wingman Tabs */
		if (!Settings[WINDOW_LOG_PROOFS_KEY][SHOW_TAB_RAIDS_NORMAL].is_null()) {
			Settings[WINDOW_LOG_PROOFS_KEY][SHOW_TAB_RAIDS_NORMAL].get_to<bool>(ShowTabRaidsNormal);
		}
		if (!Settings[WINDOW_LOG_PROOFS_KEY][SHOW_TAB_RAIDS_CM].is_null()) {
			Settings[WINDOW_LOG_PROOFS_KEY][SHOW_TAB_RAIDS_CM].get_to<bool>(ShowTabRaidsCM);
		}
		if (!Settings[WINDOW_LOG_PROOFS_KEY][SHOW_TAB_RAIDS_LM].is_null()) {
			Settings[WINDOW_LOG_PROOFS_KEY][SHOW_TAB_RAIDS_LM].get_to<bool>(ShowTabRaidsLM);
		}

		if (!Settings[WINDOW_LOG_PROOFS_KEY][SHOW_TAB_FRACTALS_CM].is_null()) {
			Settings[WINDOW_LOG_PROOFS_KEY][SHOW_TAB_FRACTALS_CM].get_to<bool>(ShowTabFractalsCM);
		}

		if (!Settings[WINDOW_LOG_PROOFS_KEY][SHOW_TAB_STRIKES_NORMAL].is_null()) {
			Settings[WINDOW_LOG_PROOFS_KEY][SHOW_TAB_STRIKES_NORMAL].get_to<bool>(ShowTabStrikesNormal);
		}
		if (!Settings[WINDOW_LOG_PROOFS_KEY][SHOW_TAB_STRIKES_CM].is_null()) {
			Settings[WINDOW_LOG_PROOFS_KEY][SHOW_TAB_STRIKES_CM].get_to<bool>(ShowTabStrikesCM);
		}
		if (!Settings[WINDOW_LOG_PROOFS_KEY][SHOW_TAB_STRIKES_LM].is_null()) {
			Settings[WINDOW_LOG_PROOFS_KEY][SHOW_TAB_STRIKES_LM].get_to<bool>(ShowTabStrikesLM);
		}

		/* Kpme Tabs */
		if (!Settings[WINDOW_LOG_PROOFS_KEY][SHOW_TAB_KPME_SUMMARY].is_null()) {
			Settings[WINDOW_LOG_PROOFS_KEY][SHOW_TAB_KPME_SUMMARY].get_to<bool>(ShowTabKpmeSummary);
		}
		if (!Settings[WINDOW_LOG_PROOFS_KEY][SHOW_TAB_KPME_RAID_TOKENS].is_null()) {
			Settings[WINDOW_LOG_PROOFS_KEY][SHOW_TAB_KPME_RAID_TOKENS].get_to<bool>(ShowTabKpmeRaidTokens);
		}
		if (!Settings[WINDOW_LOG_PROOFS_KEY][SHOW_TAB_KPME_RAID_CM_COFFERS].is_null()) {
			Settings[WINDOW_LOG_PROOFS_KEY][SHOW_TAB_KPME_RAID_CM_COFFERS].get_to<bool>(ShowTabKpmeRaidCMCoffers);
		}
		if (!Settings[WINDOW_LOG_PROOFS_KEY][SHOW_TAB_KPME_STRIKE_COFFERS].is_null()) {
			Settings[WINDOW_LOG_PROOFS_KEY][SHOW_TAB_KPME_STRIKE_COFFERS].get_to<bool>(ShowTabKpmeStrikeCoffers);
		}
		if (!Settings[WINDOW_LOG_PROOFS_KEY][SHOW_TAB_KPME_STRIKE_CM_COFFERS].is_null()) {
			Settings[WINDOW_LOG_PROOFS_KEY][SHOW_TAB_KPME_STRIKE_CM_COFFERS].get_to<bool>(ShowTabKpmeStrikeCMCoffers);
		}

		/* Columns */
		if (!Settings[WINDOW_LOG_PROOFS_KEY][COLUMN_ACCOUNT_SIZE].is_null()) {
			Settings[WINDOW_LOG_PROOFS_KEY][COLUMN_ACCOUNT_SIZE].get_to<float>(ColumnSizeAccount);
		}
		if (!Settings[WINDOW_LOG_PROOFS_KEY][COLUMN_BOSSES_SIZE].is_null()) {
			Settings[WINDOW_LOG_PROOFS_KEY][COLUMN_BOSSES_SIZE].get_to<float>(ColumnSizeBosses);
		}
		if (!Settings[WINDOW_LOG_PROOFS_KEY][COLUMN_KPME_ID_SIZE].is_null()) {
			Settings[WINDOW_LOG_PROOFS_KEY][COLUMN_KPME_ID_SIZE].get_to<float>(ColumnSizeKpmeId);
		}

		/* Data settings */
		if (!Settings[WINDOW_LOG_PROOFS_KEY][SELECTED_DATA_SOURCE].is_null()) {
			Settings[WINDOW_LOG_PROOFS_KEY][SELECTED_DATA_SOURCE].get_to<DataSource>(SelectedDataSource);
		}
		if (!Settings[WINDOW_LOG_PROOFS_KEY][INCLUDE_LINKED_ACCOUNTS].is_null()) {
			Settings[WINDOW_LOG_PROOFS_KEY][INCLUDE_LINKED_ACCOUNTS].get_to<bool>(IncludeLinkedAccounts);
		}
		if (!Settings[WINDOW_LOG_PROOFS_KEY][INCLUDE_MISSING_ACCOUNTS].is_null()) {
			Settings[WINDOW_LOG_PROOFS_KEY][INCLUDE_MISSING_ACCOUNTS].get_to<bool>(IncludeMissingAccounts);
		}

		/* Hover settings */
		if (!Settings[WINDOW_LOG_PROOFS_KEY][HOVER_ENABLED].is_null()) {
			Settings[WINDOW_LOG_PROOFS_KEY][HOVER_ENABLED].get_to<bool>(hoverEnabled);
		}
		if (!Settings[WINDOW_LOG_PROOFS_KEY][HOVER_COLOUR].is_null()) {
			Settings[WINDOW_LOG_PROOFS_KEY][HOVER_COLOUR].get_to<ImU32>(hoverColour);
			hoverColourBuffer = ImGui::ColorConvertU32ToFloat4(hoverColour);
		}

		/* Custom tabs settings */
		if (!Settings[WINDOW_LOG_PROOFS_KEY][CUSTOM_TABS_ENABLED].is_null()) {
			Settings[WINDOW_LOG_PROOFS_KEY][CUSTOM_TABS_ENABLED].get_to<bool>(CustomTabsEnabled);
		}

		/* Lazy loading settings */
		if (!Settings[WINDOW_LOG_PROOFS_KEY][LAZY_LOADING_ENABLED].is_null()) {
			Settings[WINDOW_LOG_PROOFS_KEY][LAZY_LOADING_ENABLED].get_to<bool>(LazyLoadingEnabled);
		}
		if (!Settings[WINDOW_LOG_PROOFS_KEY][CACHE_TIMEOUT_MINUTES].is_null()) {
			Settings[WINDOW_LOG_PROOFS_KEY][CACHE_TIMEOUT_MINUTES].get_to<int>(CacheTimeoutMinutes);
		}
		if (!Settings[WINDOW_LOG_PROOFS_KEY][MAX_RETRY_ATTEMPTS].is_null()) {
			Settings[WINDOW_LOG_PROOFS_KEY][MAX_RETRY_ATTEMPTS].get_to<int>(MaxRetryAttempts);
		}

		// Load custom tab configurations
		TabConfigManager::Instance().LoadFromSettings();

		// Perform migration if needed
		MigrateLegacyTabSettings();

		// Skip validation during load - will validate when needed
	}

	void Save(std::filesystem::path filePath) {
		std::scoped_lock lck(Mutex);
		SaveInternal(filePath);
	}

	void SaveInternal(std::filesystem::path filePath) {
		// Note: Caller must hold Mutex
		std::ofstream file(filePath);
		file << Settings.dump(4, ' ') << std::endl;
		file.close();
	}

	bool ShowQuickAccessShortcut = true;
	bool ShowWindowLogProofs = false;

	float MinWindowWidth = 300.0f;
	float MinWindowHeight = 100.0f;
	float MaxWindowWidth = 800.0f;
	float MaxWindowHeight = 800.0f;

	bool ShowTabRaidsNormal = true;
	bool ShowTabRaidsCM = true;
	bool ShowTabRaidsLM = true;
	bool ShowTabFractalsCM = true;
	bool ShowTabStrikesNormal = true;
	bool ShowTabStrikesCM = true;
	bool ShowTabStrikesLM = true;

	bool ShowTabKpmeSummary = true;
	bool ShowTabKpmeRaidTokens = true;
	bool ShowTabKpmeRaidCMCoffers = true;
	bool ShowTabKpmeStrikeCoffers = true;
	bool ShowTabKpmeStrikeCMCoffers = true;

	float ColumnSizeAccount = 200.0f;
	float ColumnSizeBosses = 32.0f;
	float ColumnSizeKpmeId = 32.0f;

	DataSource SelectedDataSource = WINGMAN;
	bool IncludeLinkedAccounts = false;
	bool IncludeMissingAccounts = false;

	bool hoverEnabled = true;
	ImU32 hoverColour = 4285558896;
	ImVec4 hoverColourBuffer = ImGui::ColorConvertU32ToFloat4(hoverColour);

	bool CustomTabsEnabled = false;

	bool LazyLoadingEnabled = true;
	int CacheTimeoutMinutes = 5;
	int MaxRetryAttempts = 5;

	void MigrateLegacyTabSettings() {
		if (!Settings[WINDOW_LOG_PROOFS_KEY][MIGRATION_COMPLETED].is_null() && Settings[WINDOW_LOG_PROOFS_KEY][MIGRATION_COMPLETED].get<bool>()) {
			return; // Already migrated
		}

		// Create default Wingman configuration
		ProviderTabConfig wingmanConfig;
		wingmanConfig.providerId = "Wingman";
		wingmanConfig.useCustomTabs = false;
		wingmanConfig.tabs.clear(); // Ensure empty for default mode

		// Create default KPME configuration
		ProviderTabConfig kpmeConfig;
		kpmeConfig.providerId = "KPME";
		kpmeConfig.useCustomTabs = false;
		kpmeConfig.tabs.clear(); // Ensure empty for default mode

		// Initialize TabConfigManager with default configs
		auto& tabManager = TabConfigManager::Instance();
		tabManager.SetProviderConfig("Wingman", wingmanConfig);
		tabManager.SetProviderConfig("KPME", kpmeConfig);
		tabManager.SaveToSettings();

		Settings[WINDOW_LOG_PROOFS_KEY][MIGRATION_COMPLETED] = true;
		SaveInternal(SettingsPath);
	}

	void ResetToDefaultTabs(const std::string& providerId) {
		ProviderTabConfig config;
		config.providerId = providerId;
		config.useCustomTabs = false;
		config.tabs.clear();
		TabConfigManager::Instance().SetProviderConfig(providerId, config);
		TabConfigManager::Instance().SaveAndPersist();
	}

	void EnsureProviderConfigExists(const std::string& providerId) {
		auto& tabManager = TabConfigManager::Instance();
		auto config = tabManager.GetProviderConfig(providerId);
		if (config.providerId.empty()) {
			// Provider config doesn't exist, create default
			ResetToDefaultTabs(providerId);
		}
	}
} // namespace Settings