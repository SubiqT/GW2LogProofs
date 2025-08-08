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
const char* WINDOW_AUTO_RESIZE = "WindowAutoResize";
const char* WINDOW_RESTRICT_SIZE = "WindowRestrictSize";

const char* COLUMN_ACCOUNT_SIZE = "ColumnAccountSize";
const char* COLUMN_BOSSES_SIZE = "ColumnBossesSize";
const char* COLUMN_KPME_ID_SIZE = "ColumnKpmeIdSize";
const char* BOSS_ICON_SCALE = "BossIconScale";

const char* SELECTED_DATA_SOURCE = "SelectedDataSource";
const char* LINKED_ACCOUNTS_MODE = "LinkedAccountsMode";
const char* INCLUDE_MISSING_ACCOUNTS = "IncludeMissingAccounts";

const char* HOVER_ENABLED = "HoverEnabled";
const char* HOVER_COLOUR = "HoverColour";


const char* PROVIDER_CONFIGS = "ProviderConfigs";

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
		if (!Settings[WINDOW_LOG_PROOFS_KEY][WINDOW_AUTO_RESIZE].is_null()) {
			Settings[WINDOW_LOG_PROOFS_KEY][WINDOW_AUTO_RESIZE].get_to<bool>(WindowAutoResize);
		}
		if (!Settings[WINDOW_LOG_PROOFS_KEY][WINDOW_RESTRICT_SIZE].is_null()) {
			Settings[WINDOW_LOG_PROOFS_KEY][WINDOW_RESTRICT_SIZE].get_to<bool>(WindowRestrictSize);
		}

		/* Column Sizing */
		if (!Settings[WINDOW_LOG_PROOFS_KEY][COLUMN_ACCOUNT_SIZE].is_null()) {
			Settings[WINDOW_LOG_PROOFS_KEY][COLUMN_ACCOUNT_SIZE].get_to<float>(ColumnSizeAccount);
		}
		if (!Settings[WINDOW_LOG_PROOFS_KEY][COLUMN_BOSSES_SIZE].is_null()) {
			Settings[WINDOW_LOG_PROOFS_KEY][COLUMN_BOSSES_SIZE].get_to<float>(ColumnSizeBosses);
		}
		if (!Settings[WINDOW_LOG_PROOFS_KEY][COLUMN_KPME_ID_SIZE].is_null()) {
			Settings[WINDOW_LOG_PROOFS_KEY][COLUMN_KPME_ID_SIZE].get_to<float>(ColumnSizeKpmeId);
		}
		if (!Settings[WINDOW_LOG_PROOFS_KEY][BOSS_ICON_SCALE].is_null()) {
			Settings[WINDOW_LOG_PROOFS_KEY][BOSS_ICON_SCALE].get_to<float>(BossIconScale);
		}

		/* Data settings */
		if (!Settings[WINDOW_LOG_PROOFS_KEY][SELECTED_DATA_SOURCE].is_null()) {
			Settings[WINDOW_LOG_PROOFS_KEY][SELECTED_DATA_SOURCE].get_to<DataSource>(SelectedDataSource);
		}
		if (!Settings[WINDOW_LOG_PROOFS_KEY][LINKED_ACCOUNTS_MODE].is_null()) {
			Settings[WINDOW_LOG_PROOFS_KEY][LINKED_ACCOUNTS_MODE].get_to<LinkedAccountMode>(LinkedAccountsMode);
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


		/* Cache settings */
		if (!Settings[WINDOW_LOG_PROOFS_KEY][CACHE_TIMEOUT_MINUTES].is_null() && Settings[WINDOW_LOG_PROOFS_KEY][CACHE_TIMEOUT_MINUTES].is_number_integer()) {
			int value = Settings[WINDOW_LOG_PROOFS_KEY][CACHE_TIMEOUT_MINUTES].get<int>();
			if (value >= 1 && value <= 30) {
				CacheTimeoutMinutes = value;
			}
		}
		if (!Settings[WINDOW_LOG_PROOFS_KEY][MAX_RETRY_ATTEMPTS].is_null() && Settings[WINDOW_LOG_PROOFS_KEY][MAX_RETRY_ATTEMPTS].is_number_integer()) {
			int value = Settings[WINDOW_LOG_PROOFS_KEY][MAX_RETRY_ATTEMPTS].get<int>();
			if (value >= 1 && value <= 10) {
				MaxRetryAttempts = value;
			}
		}


		TabConfigManager::Instance().LoadFromSettings();
	}

	void Save(std::filesystem::path filePath) {
		std::scoped_lock lck(Mutex);
		SaveInternal(filePath);
	}

	void SaveInternal(std::filesystem::path filePath) {

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
	bool WindowAutoResize = true;
	bool WindowRestrictSize = true;

	float ColumnSizeAccount = 200.0f;
	float ColumnSizeBosses = 32.0f;
	float ColumnSizeKpmeId = 32.0f;
	float BossIconScale = 32.0f;

	DataSource SelectedDataSource = WINGMAN;
	LinkedAccountMode LinkedAccountsMode = HIDE_LINKED;
	bool IncludeMissingAccounts = true;

	bool hoverEnabled = true;
	ImU32 hoverColour = 4285558896;
	ImVec4 hoverColourBuffer = ImGui::ColorConvertU32ToFloat4(hoverColour);


	int CacheTimeoutMinutes = 10;
	int MaxRetryAttempts = 3;

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
			ResetToDefaultTabs(providerId);
		}
	}
} // namespace Settings