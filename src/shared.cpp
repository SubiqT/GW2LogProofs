#include <chrono>

#include "shared.h"
#include "nexus/Nexus.h"

AddonConfig defaultConfig = { true };

AddonConfig Config = defaultConfig;

AddonAPI* APIDefs = nullptr;
NexusLinkData* NexusLink = nullptr;
