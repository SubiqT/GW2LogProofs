#include <filesystem>

#include "../nexus/Nexus.h"

const char* ADDON_NAME = "Log Proofs";

std::filesystem::path AddonPath;
std::filesystem::path SettingsPath;

AddonAPI* APIDefs = nullptr;
NexusLinkData* NexusLink = nullptr;
HMODULE hSelf = nullptr;

const char* KB_TOGGLE_SHOW_WINDOW_LOG_PROOFS = "KB_TOGGLE_SHOW_WINDOW_LOG_PROOFS";
