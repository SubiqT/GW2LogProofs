#include <chrono>

#include "nexus/Nexus.h"
#include "shared.h"

AddonConfig defaultConfig = { true };
AddonConfig Config = defaultConfig;

AddonAPI* APIDefs = nullptr;
NexusLinkData* NexusLink = nullptr;

std::vector<Player> players;
std::string self;