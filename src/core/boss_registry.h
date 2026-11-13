#ifndef BOSS_REGISTRY_H
#define BOSS_REGISTRY_H

#include "bosses.h"
#include "tab_config.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class IBossProvider; // Forward declaration

struct BossEntry {
	const BossInfo* boss;
	BossType type;
	ProofKind kind = ProofKind::KILL_PROOF;
};

struct BossGroup {
	std::string name;
	std::string tableName;
	std::vector<BossEntry> bosses;
	std::vector<std::string> currencies; // For summary/currency groups

	// Cached proof identifiers to avoid repeated GetProofIdentifier calls
	mutable std::vector<std::string> cachedCurrencyIds;
	mutable std::vector<std::string> cachedBossIds;
	mutable bool cacheInitialized = false;

	void InitializeCache(const IBossProvider* provider) const;
};

class IBossProvider {
public:
	virtual ~IBossProvider() = default;
	virtual std::vector<BossGroup> GetBossGroups() const = 0;
	virtual std::string GetProofIdentifier(const BossEntry& entry) const = 0;
	virtual std::string GetProofIdentifier(const std::string& currency) const = 0;

	// Custom tab support
	virtual std::vector<ProofOption> GetAvailableProofs() const = 0;
	virtual BossGroup CreateCustomBossGroup(const CustomTab& tab) const = 0;
};

class BossRegistry {
public:
	static BossRegistry& Instance();
	void RegisterProvider(const std::string& providerName, std::unique_ptr<IBossProvider> provider);
	IBossProvider* GetProvider(const std::string& providerName) const;
	std::vector<std::string> GetAvailableProviders() const;

private:
	std::unordered_map<std::string, std::unique_ptr<IBossProvider>> providers_;
};

void InitializeBossRegistry();

#endif