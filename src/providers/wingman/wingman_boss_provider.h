#ifndef WINGMAN_BOSS_PROVIDER_H
#define WINGMAN_BOSS_PROVIDER_H

#include "../../core/boss_registry.h"

class WingmanBossProvider : public IBossProvider {
public:
	std::vector<BossGroup> GetBossGroups() const override;
	std::string GetProofIdentifier(Boss boss, BossCategory category) const override;
	std::string GetProofIdentifier(const std::string& currency) const override;
};

#endif