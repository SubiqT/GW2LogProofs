#ifndef WINGMAN_PROVIDER_H
#define WINGMAN_PROVIDER_H

#include "../common/provider.h"

class WingmanProvider : public IProvider {
public:
	WingmanProvider();

	const char* Name() const override { return "Wingman"; }
	void FetchAsync(const std::string& account, FetchCallback callback) override;

	const std::vector<BossGroup>& GetBossGroups() const override { return bossGroups_; }
	std::vector<ProofOption> GetAvailableProofs() const override;
	BossGroup CreateCustomBossGroup(const CustomTab& tab) const override;

private:
	std::vector<BossGroup> bossGroups_;
};

#endif
