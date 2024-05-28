#ifndef SHARED_H
#define SHARED_H

#include <filesystem>

#include "nexus/Nexus.h"

extern const char* ADDON_NAME;

extern std::filesystem::path AddonPath;
extern std::filesystem::path SettingsPath;

extern AddonAPI* APIDefs;
extern NexusLinkData* NexusLink;
extern HMODULE hSelf;


#endif