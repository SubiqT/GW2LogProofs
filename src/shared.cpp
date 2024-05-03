#include <chrono>

#include "nexus/Nexus.h"
#include "unofficial_extras/Definitions.h"
#include "shared.h"

AddonConfig defaultConfig = { true };
AddonConfig Config = defaultConfig;

AddonAPI* APIDefs = nullptr;
NexusLinkData* NexusLink = nullptr;

ExtrasSubscriberInfoV2* ExtrasSubscriberInfo;
bool extrasLoaded;

std::vector<Player> players;
std::string self;