#ifndef SETTINGS_H
#define SETTINGS_H

#include <mutex>

#include "nlohmann/json.hpp"
using json = nlohmann::json;

extern const char* SHOW_QUICK_ACCESS_SHORTCUT;
extern const char* WINDOW_LOG_PROOFS_KEY;
extern const char* SHOW_WINDOW_LOG_PROOFS;

extern const char* MIN_WINDOW_WIDTH;
extern const char* MIN_WINDOW_HEIGHT;
extern const char* MAX_WINDOW_WIDTH;
extern const char* MAX_WINDOW_HEIGHT;

extern const char* SHOW_TAB_RAIDS_NORMAL;
extern const char* SHOW_TAB_RAIDS_CM;
extern const char* SHOW_TAB_FRACTALS_CM;
extern const char* SHOW_TAB_STRIKES_NORMAL;
extern const char* SHOW_TAB_STRIKES_CM;
extern const char* SHOW_TAB_STRIKES_LM;

extern const char* SHOW_TAB_KPME_SUMMARY;
extern const char* SHOW_TAB_KPME_RAID_TOKENS;
extern const char* SHOW_TAB_KPME_STRIKE_COFFERS;
extern const char* SHOW_TAB_KPME_STRIKE_CM_COFFERS;

extern const char* COLUMN_ACCOUNT_SIZE;
extern const char* COLUMN_BOSSES_SIZE;


namespace Settings
{
	extern std::mutex Mutex;
	extern json	Settings;

	void Load(std::filesystem::path filePath);
	void Save(std::filesystem::path filePath);

	extern bool ShowQuickAccessShortcut;
	extern bool ShowWindowLogProofs;

	extern float MinWindowWidth;
	extern float MinWindowHeight;
	extern float MaxWindowWidth;
	extern float MaxWindowHeight;

	extern bool ShowTabRaidsNormal;
	extern bool ShowTabRaidsCM;
	extern bool ShowTabFractalsCM;
	extern bool ShowTabStrikesNormal;
	extern bool ShowTabStrikesCM;
	extern bool ShowTabStrikesLM;

	extern bool ShowTabKpmeSummary;
	extern bool ShowTabKpmeRaidTokens;
	extern bool ShowTabKpmeStrikeCoffers;
	extern bool ShowTabKpmeStrikeCMCoffers;

	extern float ColumnSizeAccount;
	extern float ColumnSizeBosses;
}

#endif