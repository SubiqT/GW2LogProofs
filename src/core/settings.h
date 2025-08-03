#ifndef SETTINGS_H
#define SETTINGS_H

#include <mutex>

#include "../imgui/imgui.h"
#include "tab_config.h"

#include "../nlohmann/json.hpp"
using json = nlohmann::json;

enum DataSource {
	WINGMAN,
	KPME
};

enum LinkedAccountMode {
	HIDE_LINKED,
	COMBINE_LINKED,
	SPLIT_LINKED
};

extern const char* SHOW_QUICK_ACCESS_SHORTCUT;
extern const char* WINDOW_LOG_PROOFS_KEY;
extern const char* SHOW_WINDOW_LOG_PROOFS;

extern const char* MIN_WINDOW_WIDTH;
extern const char* MIN_WINDOW_HEIGHT;
extern const char* MAX_WINDOW_WIDTH;
extern const char* MAX_WINDOW_HEIGHT;
extern const char* WINDOW_AUTO_RESIZE;
extern const char* WINDOW_RESTRICT_SIZE;


extern const char* COLUMN_ACCOUNT_SIZE;
extern const char* COLUMN_BOSSES_SIZE;
extern const char* COLUMN_KPME_ID_SIZE;

extern const char* SELECTED_DATA_SOURCE;
extern const char* LINKED_ACCOUNTS_MODE;

extern const char* HOVER_ENABLED;
extern const char* HOVER_COLOUR;

extern const char* CUSTOM_TABS_ENABLED;
extern const char* PROVIDER_CONFIGS;

extern const char* CACHE_TIMEOUT_MINUTES;
extern const char* MAX_RETRY_ATTEMPTS;
extern const char* MAX_CONCURRENT_REQUESTS;

namespace Settings {
	extern std::mutex Mutex;
	extern json Settings;

	void Load(std::filesystem::path filePath);
	void Save(std::filesystem::path filePath);
	void SaveInternal(std::filesystem::path filePath);

	extern bool ShowQuickAccessShortcut;
	extern bool ShowWindowLogProofs;

	extern float MinWindowWidth;
	extern float MinWindowHeight;
	extern float MaxWindowWidth;
	extern float MaxWindowHeight;
	extern bool WindowAutoResize;
	extern bool WindowRestrictSize;

	extern float ColumnSizeAccount;
	extern float ColumnSizeBosses;
	extern float ColumnSizeKpmeId;

	extern DataSource SelectedDataSource;
	extern LinkedAccountMode LinkedAccountsMode;

	extern bool hoverEnabled;
	extern ImU32 hoverColour;
	extern ImVec4 hoverColourBuffer;

	// Custom tab settings
	extern bool CustomTabsEnabled;

	// Cache settings
	extern int CacheTimeoutMinutes;
	extern int MaxRetryAttempts;
	extern int MaxConcurrentRequests;

	void ResetToDefaultTabs(const std::string& providerId);
	void EnsureProviderConfigExists(const std::string& providerId);
} // namespace Settings

#endif