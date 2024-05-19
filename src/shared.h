#ifndef SHARED_H
#define SHARED_H

#include <vector>
#include <map>
#include <string>
#include <mutex>

#include "nexus/Nexus.h"
#include "unofficial_extras/Definitions.h"
#include "arcdps/arcdps.h"

extern const char* ADDON_NAME;
extern const char* SHOW_ADDON_KEYBIND;

typedef struct AddonConfig {
    bool showWindow;
} AddonConfig;
extern AddonConfig Config;

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


struct SquadUpdate
{
    UserInfo* UserInfo;
    uint64_t UsersCount;
};

#endif