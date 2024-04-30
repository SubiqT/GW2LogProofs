#ifndef SHARED_H
#define SHARED_H

#include <vector>
#include <map>
#include <string>

#include "nexus/Nexus.h"

extern AddonAPI* APIDefs;
extern NexusLinkData* NexusLink;

struct Player {
    std::string account;
    std::vector<std::string> characters;
    std::vector<std::string> groups;
    std::map<std::string, std::map<std::string, int>> kp;
};

#endif