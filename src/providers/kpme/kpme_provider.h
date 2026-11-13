#ifndef KPME_PROVIDER_H
#define KPME_PROVIDER_H

#include "../common/provider.h"

class KpmeProvider : public IProvider {
public:
	KpmeProvider();

	const char* Name() const override { return "KPME"; }
	const char* MissingDataDescription() const override { return "No killproof.me profile, or the profile is private"; }
	bool SupportsLinkedAccounts() const override { return true; }
	bool ShowsProfileId() const override { return true; }
	void FetchAsync(const std::string& account, FetchCallback callback) override;

	const std::vector<BossGroup>& GetBossGroups() const override { return bossGroups_; }
	std::vector<ProofOption> GetAvailableProofs() const override;
	BossGroup CreateCustomBossGroup(const CustomTab& tab) const override;

private:
	std::vector<BossGroup> bossGroups_;
};

#endif
