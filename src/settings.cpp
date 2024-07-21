#include "settings.h"

#include "shared.h"

#include <filesystem>
#include <fstream>
#include <format>

const char* SHOW_QUICK_ACCESS_SHORTCUT = "ShowQuickAccessShortcut";
const char* WINDOW_LOG_PROOFS_KEY = "WindowLogProofs";
const char* SHOW_WINDOW_LOG_PROOFS = "ShowWindow";

const char* MIN_WINDOW_WIDTH = "MinWindowWidth";
const char* MIN_WINDOW_HEIGHT = "MinWindowHeight";
const char* MAX_WINDOW_WIDTH = "MaxWindowWidth";
const char* MAX_WINDOW_HEIGHT = "MaxWindowHeight";

const char* SHOW_TAB_RAIDS_NORMAL = "ShowTabRaidsNormal";
const char* SHOW_TAB_RAIDS_CM = "ShowTabRaidsCM";
const char* SHOW_TAB_FRACTALS_CM = "ShowTabFractalsCM";
const char* SHOW_TAB_STRIKES_NORMAL = "ShowTabStrikesNormal";
const char* SHOW_TAB_STRIKES_CM = "ShowTabStrikesCM";
const char* SHOW_TAB_STRIKES_LM = "ShowTabStrikesLM";

const char* SHOW_TAB_KPME_SUMMARY = "ShowTabKpmeSummary";
const char* SHOW_TAB_KPME_RAID_TOKENS = "ShowTabKpmeRaidTokens";
const char* SHOW_TAB_KPME_STRIKE_COFFERS = "ShowTabKpmeStrikeCoffers";
const char* SHOW_TAB_KPME_STRIKE_CM_COFFERS = "ShowTabKpmeStrikeCMCoffers";

const char* COLUMN_ACCOUNT_SIZE = "ColumnAccountSize";
const char* COLUMN_BOSSES_SIZE = "ColumnBossesSize";
const char* COLUMN_KPME_ID_SIZE = "ColumnKpmeIdSize";

const char* SELECTED_DATA_SOURCE = "SelectedDataSource";
const char* INCLUDE_LINKED_ACCOUNTS = "IncludeLinkedAccounts";
const char* INCLUDE_MISSING_ACCOUNTS = "IncludeMissingAccounts";


namespace Settings
{
	std::mutex Mutex;
	json Settings = json::object();

	void Load(std::filesystem::path filePath) {
		if (!std::filesystem::exists(filePath)) return;

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
	}
	void Save(std::filesystem::path filePath) {
		std::scoped_lock lck(Mutex);
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
	bool ShowTabFractalsCM = true;
	bool ShowTabStrikesNormal = true;
	bool ShowTabStrikesCM = true;
	bool ShowTabStrikesLM = true;

	bool ShowTabKpmeSummary = true;
	bool ShowTabKpmeRaidTokens = true;
	bool ShowTabKpmeStrikeCoffers = true;
	bool ShowTabKpmeStrikeCMCoffers = true;

	float ColumnSizeAccount = 200.0f;
	float ColumnSizeBosses = 32.0f;
	float ColumnSizeKpmeId = 32.0f;

	DataSource SelectedDataSource = WINGMAN;
	bool IncludeLinkedAccounts = false;
	bool IncludeMissingAccounts = false;
}