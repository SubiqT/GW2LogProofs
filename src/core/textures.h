#ifndef TEXTURES_H
#define TEXTURES_H

#include <string>

#include "../nexus/Nexus.h"
#include "bosses.h"

Texture* GetBossTexture(const BossInfo& boss);
Texture* GetCurrencyTexture(const std::string& name);
Texture* GetCommanderTexture();

#endif
