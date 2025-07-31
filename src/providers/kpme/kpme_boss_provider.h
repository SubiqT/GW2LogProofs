#ifndef KPME_BOSS_PROVIDER_H
#define KPME_BOSS_PROVIDER_H

#include "../../core/boss_registry.h"

class KpmeBossProvider : public IBossProvider {
public:
	std::vector<BossGroup> GetBossGroups() const override;
	std::string GetProofIdentifier(Boss boss, BossCategory category) const override;
	std::string GetProofIdentifier(const std::string& currency) const override;
};

std::string GetKpMeBossToken(Boss boss);
std::string GetKpMeBossCoffer(Boss boss);

#endif