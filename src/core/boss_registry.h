#ifndef BOSS_REGISTRY_H
#define BOSS_REGISTRY_H

#include "bosses.h"
#include "tab_config.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class IBossProvider; // Forward declaration

enum class BossCategory {
	RAID_NORMAL,
	RAID_CM,
	RAID_LM,
	FRACTAL_CM,
	STRIKE_NORMAL,
	STRIKE_CM,
	STRIKE_LM,
	SUMMARY
};

struct BossEntry {
	Boss boss;
	BossType type;
};

struct BossGroup {
	std::string name;
	std::string tableName;
	BossCategory category;
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
	virtual std::string GetProofIdentifier(Boss boss, BossCategory category) const = 0;
	virtual std::string GetProofIdentifier(Boss boss, BossType type) const { return GetProofIdentifier(boss, BossCategory::SUMMARY); }
	virtual std::string GetProofIdentifier(const std::string& currency) const = 0;

	// Custom tab support
	virtual std::vector<ProofOption> GetAvailableProofs() const = 0;
	virtual BossGroup CreateCustomBossGroup(const CustomTab& tab) const = 0;
	virtual bool SupportsCustomTabs() const { return true; }
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

#endif