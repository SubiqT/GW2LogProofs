#ifndef KPME_PROVIDER_H
#define KPME_PROVIDER_H

#include "../common/provider.h"
#include "kpme_client.h"

class KpmeProvider : public IProvider {
public:
	KpmeProvider();

	const char* Name() const override { return "KPME"; }
	bool SupportsLinkedAccounts() const override { return true; }
	bool ShowsProfileId() const override { return true; }
	void LoadPlayerDataAsync(const std::string& account, PlayerProofsCallback callback) override;

	const std::vector<BossGroup>& GetBossGroups() const override { return bossGroups_; }
	std::vector<ProofOption> GetAvailableProofs() const override;
	BossGroup CreateCustomBossGroup(const CustomTab& tab) const override;

private:
	Kpme::KpmeClient client_;
	std::vector<BossGroup> bossGroups_;
};

#endif
