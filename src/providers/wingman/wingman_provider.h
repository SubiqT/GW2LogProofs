#ifndef WINGMAN_PROVIDER_H
#define WINGMAN_PROVIDER_H

#include "../common/provider.h"
#include "wingman_client.h"

class WingmanProvider : public IProvider {
public:
	WingmanProvider();

	const char* Name() const override { return "Wingman"; }
	void LoadPlayerDataAsync(const std::string& account, PlayerProofsCallback callback) override;

	const std::vector<BossGroup>& GetBossGroups() const override { return bossGroups_; }
	std::vector<ProofOption> GetAvailableProofs() const override;
	BossGroup CreateCustomBossGroup(const CustomTab& tab) const override;

private:
	Wingman::WingmanClient client_;
	std::vector<BossGroup> bossGroups_;
};

#endif
