#ifndef SHARED_H
#define SHARED_H

#include <vector>
#include <map>
#include <string>
#include <filesystem>

#include "nexus/Nexus.h"

#include "arcdps/arcdps.h"

extern const char* ADDON_NAME;

extern std::filesystem::path AddonPath;
extern std::filesystem::path SettingsPath;

extern AddonAPI* APIDefs;
extern NexusLinkData* NexusLink;

struct Player {
    std::string account;
    std::vector<std::string> characters;
    std::vector<std::string> groups;
    std::map<std::string, std::map<std::string, int>> kp;
};
extern std::vector<Player> players;
extern std::string selfName;

#endif