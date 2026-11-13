#include "settings.h"
#include "shared.h"
#include "tab_config.h"

#include <algorithm>
#include <format>
#include <fstream>

#include "../nlohmann/json.hpp"

using json = nlohmann::json;

namespace {
	constexpr const char* SHOW_QUICK_ACCESS_SHORTCUT = "ShowQuickAccessShortcut";
	constexpr const char* WINDOW_LOG_PROOFS_KEY = "WindowLogProofs";
	constexpr const char* SHOW_WINDOW_LOG_PROOFS = "ShowWindow";
	constexpr const char* MIN_WINDOW_WIDTH = "MinWindowWidth";
	constexpr const char* MIN_WINDOW_HEIGHT = "MinWindowHeight";
	constexpr const char* MAX_WINDOW_WIDTH = "MaxWindowWidth";
	constexpr const char* MAX_WINDOW_HEIGHT = "MaxWindowHeight";
	constexpr const char* WINDOW_AUTO_RESIZE = "WindowAutoResize";
	constexpr const char* WINDOW_RESTRICT_SIZE = "WindowRestrictSize";
	constexpr const char* COLUMN_ACCOUNT_SIZE = "ColumnAccountSize";
	constexpr const char* COLUMN_BOSSES_SIZE = "ColumnBossesSize";
	constexpr const char* COLUMN_KPME_ID_SIZE = "ColumnKpmeIdSize";
	constexpr const char* BOSS_ICON_SCALE = "BossIconScale";
	constexpr const char* SELECTED_DATA_SOURCE = "SelectedDataSource";
	constexpr const char* LINKED_ACCOUNTS_MODE = "LinkedAccountsMode";
	constexpr const char* INCLUDE_MISSING_ACCOUNTS = "IncludeMissingAccounts";
	constexpr const char* HOVER_ENABLED = "HoverEnabled";
	constexpr const char* HOVER_COLOUR = "HoverColour";
	constexpr const char* CACHE_TIMEOUT_MINUTES = "CacheTimeoutMinutes";
	constexpr const char* MAX_RETRY_ATTEMPTS = "MaxRetryAttempts";
	constexpr const char* PROVIDER_CONFIGS = "ProviderConfigs";

	std::filesystem::path settingsPath;
	json document = json::object();

	template <typename T>
	void Read(const json& object, const char* key, T& value) {
		auto it = object.find(key);
		if (it == object.end()) return;
		try {
			value = it->get<T>();
		} catch (const json::exception& e) {
			APIDefs->Log(ELogLevel_WARNING, ADDON_NAME, std::format("Ignoring invalid setting {}: {}", key, e.what()).c_str());
		}
	}

	template <typename T, typename Range>
	void ReadClamped(const json& object, const char* key, T& value, Range range) {
		Read(object, key, value);
		value = std::clamp(value, range.min, range.max);
	}

	template <typename Enum>
	void ReadEnum(const json& object, const char* key, Enum& value, Enum last) {
		int raw = value;
		Read(object, key, raw);
		if (raw >= 0 && raw <= last) value = static_cast<Enum>(raw);
	}

	json& WindowSettings() {
		json& window = document[WINDOW_LOG_PROOFS_KEY];
		if (!window.is_object()) window = json::object();
		return window;
	}
} // namespace

namespace Settings {
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

	void Load(const std::filesystem::path& filePath) {
		settingsPath = filePath;
		if (std::filesystem::exists(filePath)) {
			std::ifstream file(filePath);
			document = json::parse(file, nullptr, false);
			if (!document.is_object()) {
				APIDefs->Log(ELogLevel_CRITICAL, ADDON_NAME, "settings.json is not a valid JSON object; using default settings.");
				document = json::object();
			}
		}

		Read(document, SHOW_QUICK_ACCESS_SHORTCUT, ShowQuickAccessShortcut);

		const json& window = WindowSettings();
		Read(window, SHOW_WINDOW_LOG_PROOFS, ShowWindowLogProofs);
		ReadClamped(window, MIN_WINDOW_WIDTH, MinWindowWidth, WINDOW_SIZE_RANGE);
		ReadClamped(window, MIN_WINDOW_HEIGHT, MinWindowHeight, WINDOW_SIZE_RANGE);
		ReadClamped(window, MAX_WINDOW_WIDTH, MaxWindowWidth, WINDOW_SIZE_RANGE);
		ReadClamped(window, MAX_WINDOW_HEIGHT, MaxWindowHeight, WINDOW_SIZE_RANGE);
		MaxWindowWidth = (std::max)(MaxWindowWidth, MinWindowWidth);
		MaxWindowHeight = (std::max)(MaxWindowHeight, MinWindowHeight);
		Read(window, WINDOW_AUTO_RESIZE, WindowAutoResize);
		Read(window, WINDOW_RESTRICT_SIZE, WindowRestrictSize);

		ReadClamped(window, COLUMN_ACCOUNT_SIZE, ColumnSizeAccount, ACCOUNT_COLUMN_RANGE);
		ReadClamped(window, COLUMN_BOSSES_SIZE, ColumnSizeBosses, PROOF_COLUMN_RANGE);
		ReadClamped(window, COLUMN_KPME_ID_SIZE, ColumnSizeKpmeId, PROOF_COLUMN_RANGE);
		ReadClamped(window, BOSS_ICON_SCALE, BossIconScale, PROOF_COLUMN_RANGE);

		ReadEnum(window, SELECTED_DATA_SOURCE, SelectedDataSource, KPME);
		ReadEnum(window, LINKED_ACCOUNTS_MODE, LinkedAccountsMode, SPLIT_LINKED);
		Read(window, INCLUDE_MISSING_ACCOUNTS, IncludeMissingAccounts);

		Read(window, HOVER_ENABLED, hoverEnabled);
		Read(window, HOVER_COLOUR, hoverColour);
		hoverColourBuffer = ImGui::ColorConvertU32ToFloat4(hoverColour);

		ReadClamped(window, CACHE_TIMEOUT_MINUTES, CacheTimeoutMinutes, CACHE_TIMEOUT_MINUTES_RANGE);
		ReadClamped(window, MAX_RETRY_ATTEMPTS, MaxRetryAttempts, RETRY_ATTEMPTS_RANGE);

		if (auto configs = window.find(PROVIDER_CONFIGS); configs != window.end()) {
			TabConfigManager::Instance().Load(*configs);
		}
	}

	void Save() {
		document[SHOW_QUICK_ACCESS_SHORTCUT] = ShowQuickAccessShortcut;

		json& window = WindowSettings();
		window[SHOW_WINDOW_LOG_PROOFS] = ShowWindowLogProofs;
		window[MIN_WINDOW_WIDTH] = MinWindowWidth;
		window[MIN_WINDOW_HEIGHT] = MinWindowHeight;
		window[MAX_WINDOW_WIDTH] = MaxWindowWidth;
		window[MAX_WINDOW_HEIGHT] = MaxWindowHeight;
		window[WINDOW_AUTO_RESIZE] = WindowAutoResize;
		window[WINDOW_RESTRICT_SIZE] = WindowRestrictSize;
		window[COLUMN_ACCOUNT_SIZE] = ColumnSizeAccount;
		window[COLUMN_BOSSES_SIZE] = ColumnSizeBosses;
		window[COLUMN_KPME_ID_SIZE] = ColumnSizeKpmeId;
		window[BOSS_ICON_SCALE] = BossIconScale;
		window[SELECTED_DATA_SOURCE] = SelectedDataSource;
		window[LINKED_ACCOUNTS_MODE] = LinkedAccountsMode;
		window[INCLUDE_MISSING_ACCOUNTS] = IncludeMissingAccounts;
		window[HOVER_ENABLED] = hoverEnabled;
		window[HOVER_COLOUR] = hoverColour;
		window[CACHE_TIMEOUT_MINUTES] = CacheTimeoutMinutes;
		window[MAX_RETRY_ATTEMPTS] = MaxRetryAttempts;
		window[PROVIDER_CONFIGS] = TabConfigManager::Instance().ToJson();

		std::ofstream file(settingsPath);
		file << document.dump(4, ' ') << std::endl;
	}

} // namespace Settings
