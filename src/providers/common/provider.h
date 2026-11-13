#ifndef PROVIDER_H
#define PROVIDER_H

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "../../core/boss_group.h"
#include "../../core/tab_config.h"

using ProofCounts = std::unordered_map<std::string, int>;

struct LinkedAccountProofs {
	std::string accountName;
	ProofCounts proofs;
};

struct PlayerProofs {
	std::string profileId;
	std::string profileUrl;
	ProofCounts own;
	// Own proofs plus those of linked accounts; equal to `own` for providers without linked accounts.
	ProofCounts combined;
	std::vector<LinkedAccountProofs> linkedAccounts;
};

using PlayerProofsCallback = std::function<void(std::shared_ptr<const PlayerProofs>)>;

class IProvider {
public:
	virtual ~IProvider() = default;
	virtual const char* Name() const = 0;
	virtual bool SupportsLinkedAccounts() const { return false; }
	virtual bool ShowsProfileId() const { return false; }

	virtual void LoadPlayerDataAsync(const std::string& account, PlayerProofsCallback callback) = 0;

	virtual const std::vector<BossGroup>& GetBossGroups() const = 0;
	virtual std::vector<ProofOption> GetAvailableProofs() const = 0;
	virtual BossGroup CreateCustomBossGroup(const CustomTab& tab) const = 0;
};

#endif
