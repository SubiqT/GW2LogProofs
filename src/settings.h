#ifndef SETTINGS_H
#define SETTINGS_H

#include <mutex>

#include "nlohmann/json.hpp"
using json = nlohmann::json;

extern const char* SHOW_WINDOW_LOG_PROOFS;

extern const char* SHOW_TAB_RAIDS_NORMAL;
extern const char* SHOW_TAB_RAIDS_CM;
extern const char* SHOW_TAB_FRACTALS_NORMAL;
extern const char* SHOW_TAB_FRACTALS_CM;
extern const char* SHOW_TAB_STRIKES_NORMAL;
extern const char* SHOW_TAB_STRIKES_CM;

namespace Settings
{
	extern std::mutex Mutex;
	extern json	Settings;

	void Load(std::filesystem::path filePath);
	void Save(std::filesystem::path filePath);

	extern bool ShowWindowLogProofs;

	extern bool ShowTabRaidsNormal;
	extern bool ShowTabRaidsCM;
	extern bool ShowTabFractalsNormal;
	extern bool ShowTabFractalsCM;
	extern bool ShowTabStrikesNormal;
	extern bool ShowTabStrikesCM;
}

#endif