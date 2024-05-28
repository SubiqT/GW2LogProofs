#include "settings.h"

#include "shared.h"

#include <filesystem>
#include <fstream>
#include <format>


const char* WINDOW_LOG_PROOFS_KEY = "WindowLogProofs";
const char* SHOW_WINDOW_LOG_PROOFS = "ShowWindow";

const char* MIN_WINDOW_WIDTH = "MinWindowWidth";
const char* MIN_WINDOW_HEIGHT = "MinWindowHeight";
const char* MAX_WINDOW_WIDTH = "MaxWindowWidth";
const char* MAX_WINDOW_HEIGHT = "MaxWindowHeight";

const char* SHOW_TAB_RAIDS_NORMAL = "ShowTabRaidsNormal";
const char* SHOW_TAB_RAIDS_CM = "ShowTabRaidsCM";
const char* SHOW_TAB_FRACTALS_NORMAL = "ShowTabFractalsNormal";
const char* SHOW_TAB_FRACTALS_CM = "ShowTabFractalsCM";
const char* SHOW_TAB_STRIKES_NORMAL = "ShowTabStrikesNormal";
const char* SHOW_TAB_STRIKES_CM = "ShowTabStrikesCM";
const char* SHOW_TAB_STRIKES_LM = "ShowTabStrikesLM";

const char* COLUMN_ACCOUNT_SIZE = "ColumnAccountSize";
const char* COLUMN_BOSSES_SIZE = "ColumnBossesSize";


namespace Settings
{
	std::mutex Mutex;
	json Settings = json::object();

	void Load(std::filesystem::path filePath)
	{
		if (!std::filesystem::exists(filePath)) { return; }

		Settings::Mutex.lock();
		{
			try
			{
				std::ifstream file(filePath);
				Settings = json::parse(file);
				file.close();
			}
			catch (json::parse_error& ex)
			{
				APIDefs->Log(ELogLevel_WARNING, ADDON_NAME, std::format("Failed to parse settings.json file. Exception details: {}", ex.what()).c_str());
			}
		}
		Settings::Mutex.unlock();

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

		/* Tabs */
		if (!Settings[WINDOW_LOG_PROOFS_KEY][SHOW_TAB_RAIDS_NORMAL].is_null()) {
			Settings[WINDOW_LOG_PROOFS_KEY][SHOW_TAB_RAIDS_NORMAL].get_to<bool>(ShowTabRaidsNormal);
		}
		if (!Settings[WINDOW_LOG_PROOFS_KEY][SHOW_TAB_RAIDS_CM].is_null()) {
			Settings[WINDOW_LOG_PROOFS_KEY][SHOW_TAB_RAIDS_CM].get_to<bool>(ShowTabRaidsCM);
		}

		if (!Settings[WINDOW_LOG_PROOFS_KEY][SHOW_TAB_FRACTALS_NORMAL].is_null()) {
			Settings[WINDOW_LOG_PROOFS_KEY][SHOW_TAB_FRACTALS_NORMAL].get_to<bool>(ShowTabFractalsNormal);
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

		/* Columns */
		if (!Settings[WINDOW_LOG_PROOFS_KEY][COLUMN_ACCOUNT_SIZE].is_null()) {
			Settings[WINDOW_LOG_PROOFS_KEY][COLUMN_ACCOUNT_SIZE].get_to<float>(ColumnSizeAccount);
		}
		if (!Settings[WINDOW_LOG_PROOFS_KEY][COLUMN_BOSSES_SIZE].is_null()) {
			Settings[WINDOW_LOG_PROOFS_KEY][COLUMN_BOSSES_SIZE].get_to<float>(ColumnSizeBosses);
		}

	}
	void Save(std::filesystem::path filePath)
	{
		Settings::Mutex.lock();
		{
			std::ofstream file(filePath);
			file << Settings.dump(4, ' ') << std::endl;
			file.close();
		}
		Settings::Mutex.unlock();
	}

	bool ShowWindowLogProofs = false;

	float MinWindowWidth = 300.0f;
	float MinWindowHeight = 100.0f;
	float MaxWindowWidth = 800.0f;
	float MaxWindowHeight = 800.0f;

	bool ShowTabRaidsNormal = true;
	bool ShowTabRaidsCM = true;
	bool ShowTabFractalsNormal = true;
	bool ShowTabFractalsCM = true;
	bool ShowTabStrikesNormal = true;
	bool ShowTabStrikesCM = true;
	bool ShowTabStrikesLM = true;

	float ColumnSizeAccount = 200.0f;
	float ColumnSizeBosses = 32.0f;
}