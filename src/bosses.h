#ifndef BOSSES_H
#define BOSSES_H

#include <vector>

#include "nexus/Nexus.h"

extern enum Boss;

extern std::vector<Boss> sortedRaidBosses;
extern std::vector<Boss> sortedRaidCmBosses;
extern std::vector<Boss> sortedFractalBosses;
extern std::vector<Boss> sortedFractalCMBosses;
extern std::vector<Boss> sortedStrikeBosses;
extern std::vector<Boss> sortedStrikeCMBosses;

const char* GetBossName(Boss boss);
Texture* GetBossTexture(Boss boss);

#endif