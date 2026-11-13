#ifndef PROVIDER_H
#define PROVIDER_H

#include <any>
#include <functional>
#include <map>
#include <string>
#include <vector>

#include "../../core/boss_group.h"
#include "../../core/tab_config.h"

struct ProofData {
	std::string identifier;
	int amount;
};

struct LinkedAccountData {
	std::string accountName;
	std::map<std::string, ProofData> proofs;
};

struct PlayerProofData {
	std::string accountName;
	std::string profileId;
	std::string profileUrl;
	std::map<std::string, ProofData> proofs;
	std::vector<LinkedAccountData> linkedAccounts;
	std::any rawData; // Provider-specific raw data for dynamic computation
};

class IProvider {
public:
	virtual ~IProvider() = default;
	virtual const char* Name() const = 0;
	virtual bool SupportsLinkedAccounts() const { return false; }
	virtual bool ShowsProfileId() const { return false; }

	virtual void LoadPlayerDataAsync(const std::string& account, std::function<void(const PlayerProofData&)> callback) = 0;
	virtual PlayerProofData ComputeProofsFromRawData(const PlayerProofData& rawData, bool includeLinkedAccounts) const { return rawData; }

	virtual const std::vector<BossGroup>& GetBossGroups() const = 0;
	virtual std::vector<ProofOption> GetAvailableProofs() const = 0;
	virtual BossGroup CreateCustomBossGroup(const CustomTab& tab) const = 0;
};

#endif
