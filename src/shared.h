#ifndef SHARED_H
#define SHARED_H

#include <vector>
#include <map>
#include <string>

#include "nexus/Nexus.h"

typedef struct AddonConfig {
    bool showWindow;
} AddonConfig;

extern AddonConfig Config;

extern AddonAPI* APIDefs;
extern NexusLinkData* NexusLink;
extern bool extrasLoaded;

struct Player {
    std::string account;
    std::vector<std::string> characters;
    std::vector<std::string> groups;
    std::map<std::string, std::map<std::string, int>> kp;
};

extern std::vector<Player> players;
extern std::string self;

enum Boss {
    ValeGuardian = 15438,
    Gorseval = 15429,
    Sabetha = 15375,
    Slothasor = 16123,
    BanditTrio = 16088,
    Matthias = 16115,
    Escort  = 16253,
    KeepConstruct = 16235,
    TwistedCastle = 16247,
    Xera = 16246,
    Cairn  = 17194,
    MursaatOverseer = 17172,
    Samarog = 17188,
    Deimos  = 17154,
    SoullessHorror = 19767,
    RiverOfSouls = 19828,
    BrokenKing = 19691,
    EaterOfSouls  = 19536,
    Eyes = 19651,
    Dhuum  = 19450,
    ConjuredAmalgamate = 43974,
    TwinLargos = 21105,
    Qadim = 20934,
    Adina = 22006,
    Sabir = 21964,
    QadimThePeerless = 22000
};

#endif