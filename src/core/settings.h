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

extern const char* SHOW_QUICK_ACCESS_SHORTCUT;
extern const char* WINDOW_LOG_PROOFS_KEY;
extern const char* SHOW_WINDOW_LOG_PROOFS;

extern const char* MIN_WINDOW_WIDTH;
extern const char* MIN_WINDOW_HEIGHT;
extern const char* MAX_WINDOW_WIDTH;
extern const char* MAX_WINDOW_HEIGHT;

extern const char* SHOW_TAB_RAIDS_NORMAL;
extern const char* SHOW_TAB_RAIDS_CM;
extern const char* SHOW_TAB_RAIDS_LM;
extern const char* SHOW_TAB_FRACTALS_CM;
extern const char* SHOW_TAB_STRIKES_NORMAL;
extern const char* SHOW_TAB_STRIKES_CM;
extern const char* SHOW_TAB_STRIKES_LM;

extern const char* SHOW_TAB_KPME_SUMMARY;
extern const char* SHOW_TAB_KPME_RAID_TOKENS;
extern const char* SHOW_TAB_KPME_RAID_CM_COFFERS;
extern const char* SHOW_TAB_KPME_STRIKE_COFFERS;
extern const char* SHOW_TAB_KPME_STRIKE_CM_COFFERS;

extern const char* COLUMN_ACCOUNT_SIZE;
extern const char* COLUMN_BOSSES_SIZE;
extern const char* COLUMN_KPME_ID_SIZE;

extern const char* SELECTED_DATA_SOURCE;
extern const char* INCLUDE_LINKED_ACCOUNTS;
extern const char* INCLUDE_MISSING_ACCOUNTS;

extern const char* HOVER_ENABLED;
extern const char* HOVER_COLOUR;

extern const char* CUSTOM_TABS_ENABLED;
extern const char* PROVIDER_CONFIGS;
extern const char* MIGRATION_COMPLETED;

extern const char* LAZY_LOADING_ENABLED;
extern const char* CACHE_TIMEOUT_MINUTES;
extern const char* MAX_RETRY_ATTEMPTS;

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

	extern bool ShowTabRaidsNormal;
	extern bool ShowTabRaidsCM;
	extern bool ShowTabRaidsLM;
	extern bool ShowTabFractalsCM;
	extern bool ShowTabStrikesNormal;
	extern bool ShowTabStrikesCM;
	extern bool ShowTabStrikesLM;

	extern bool ShowTabKpmeSummary;
	extern bool ShowTabKpmeRaidTokens;
	extern bool ShowTabKpmeRaidCMCoffers;
	extern bool ShowTabKpmeStrikeCoffers;
	extern bool ShowTabKpmeStrikeCMCoffers;

	extern float ColumnSizeAccount;
	extern float ColumnSizeBosses;
	extern float ColumnSizeKpmeId;

	extern DataSource SelectedDataSource;
	extern bool IncludeLinkedAccounts;
	extern bool IncludeMissingAccounts;

	extern bool hoverEnabled;
	extern ImU32 hoverColour;
	extern ImVec4 hoverColourBuffer;

	// Custom tab settings
	extern bool CustomTabsEnabled;

	// Lazy loading settings
	extern bool LazyLoadingEnabled;
	extern int CacheTimeoutMinutes;
	extern int MaxRetryAttempts;

	// Migration functions
	void MigrateLegacyTabSettings();
	void ResetToDefaultTabs(const std::string& providerId);
	void EnsureProviderConfigExists(const std::string& providerId);
} // namespace Settings

#endif