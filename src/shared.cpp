#include <chrono>
#include <mutex>
#include <filesystem>

#include "nexus/Nexus.h"
#include "shared.h"

const char* ADDON_NAME = "Log Proofs";

std::filesystem::path AddonPath;
std::filesystem::path SettingsPath;

AddonAPI* APIDefs = nullptr;
NexusLinkData* NexusLink = nullptr;
HMODULE hSelf = nullptr;

std::vector<Player> players;
std::string selfName;
