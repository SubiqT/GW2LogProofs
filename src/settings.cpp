#include "settings.h"

#include "shared.h"

#include <filesystem>
#include <fstream>
#include <format>

const char* SHOW_WINDOW_LOG_PROOFS = "ShowWindowLogProofs";

const char* SHOW_TAB_RAIDS_NORMAL = "ShowTabRaidsNormal";
const char* SHOW_TAB_RAIDS_CM = "ShowTabRaidsCM";
const char* SHOW_TAB_FRACTALS_NORMAL = "ShowTabFractalsNormal";
const char* SHOW_TAB_FRACTALS_CM = "ShowTabFractalsCM";
const char* SHOW_TAB_STRIKES_NORMAL = "ShowTabStrikesNormal";
const char* SHOW_TAB_STRIKES_CM = "ShowTabStrikesCM";


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

		if (!Settings[SHOW_WINDOW_LOG_PROOFS].is_null()) {
			Settings[SHOW_WINDOW_LOG_PROOFS].get_to<bool>(ShowWindowLogProofs);
		}

		if (!Settings[SHOW_TAB_RAIDS_NORMAL].is_null()) {
			Settings[SHOW_TAB_RAIDS_NORMAL].get_to<bool>(ShowTabRaidsNormal);
		}
		if (!Settings[SHOW_TAB_RAIDS_CM].is_null()) {
			Settings[SHOW_TAB_RAIDS_CM].get_to<bool>(ShowTabRaidsCM);
		}

		if (!Settings[SHOW_TAB_FRACTALS_NORMAL].is_null()) {
			Settings[SHOW_TAB_FRACTALS_NORMAL].get_to<bool>(ShowTabFractalsNormal);
		}
		if (!Settings[SHOW_TAB_FRACTALS_CM].is_null()) {
			Settings[SHOW_TAB_FRACTALS_CM].get_to<bool>(ShowTabFractalsCM);
		}

		if (!Settings[SHOW_TAB_STRIKES_NORMAL].is_null()) {
			Settings[SHOW_TAB_STRIKES_NORMAL].get_to<bool>(ShowTabStrikesNormal);
		}
		if (!Settings[SHOW_TAB_STRIKES_CM].is_null()) {
			Settings[SHOW_TAB_STRIKES_CM].get_to<bool>(ShowTabStrikesCM);
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

	bool ShowTabRaidsNormal = true;
	bool ShowTabRaidsCM = true;
	bool ShowTabFractalsNormal = true;
	bool ShowTabFractalsCM = true;
	bool ShowTabStrikesNormal = true;
	bool ShowTabStrikesCM = true;
}