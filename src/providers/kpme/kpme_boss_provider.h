#ifndef KPME_BOSS_PROVIDER_H
#define KPME_BOSS_PROVIDER_H

#include "../../core/boss_registry.h"

class KpmeBossProvider : public IBossProvider {
public:
	std::vector<BossGroup> GetBossGroups() const override;
	std::string GetProofIdentifier(Boss boss, BossType type) const override;
	std::string GetProofIdentifier(const std::string& currency) const override;

	// Custom tab support
	std::vector<ProofOption> GetAvailableProofs() const override;
	BossGroup CreateCustomBossGroup(const CustomTab& tab) const override;
};

std::string GetKpMeBossIdentifier(Boss boss);

#endif