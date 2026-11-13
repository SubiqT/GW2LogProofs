#ifndef PROVIDER_H
#define PROVIDER_H

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "../../core/boss_group.h"
#include "../../core/tab_config.h"

struct ProofAmount {
	int items = 0;
	// Boss coffers that each stand in for several of the boss's tokens.
	int coffers = 0;

	int Total(int itemsPerCoffer) const { return items + coffers * itemsPerCoffer; }
};

using ProofCounts = std::unordered_map<std::string, ProofAmount>;

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

enum class FetchStatus {
	FOUND,
	// The service has no data for the account; retried only after the cache timeout.
	NOT_FOUND,
	// Network, server or response errors; retried with backoff.
	FAILED
};

struct FetchResult {
	FetchStatus status;
	std::shared_ptr<const PlayerProofs> proofs; // Set only when status is FOUND.
};

using FetchCallback = std::function<void(const FetchResult&)>;

class IProvider {
public:
	virtual ~IProvider() = default;
	virtual const char* Name() const = 0;
	virtual bool SupportsLinkedAccounts() const { return false; }
	virtual bool ShowsProfileId() const { return false; }
	// Shown when the service has no proofs for an account.
	virtual const char* MissingDataDescription() const = 0;

	virtual void FetchAsync(const std::string& account, FetchCallback callback) = 0;

	virtual const std::vector<BossGroup>& GetBossGroups() const = 0;
	virtual std::vector<ProofOption> GetAvailableProofs() const = 0;
	virtual BossGroup CreateCustomBossGroup(const CustomTab& tab) const = 0;
};

#endif
