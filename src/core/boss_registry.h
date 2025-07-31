#ifndef BOSS_REGISTRY_H
#define BOSS_REGISTRY_H

#include "bosses.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

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

struct BossGroup {
	std::string name;
	std::string tableName;
	BossCategory category;
	std::vector<Boss> bosses;
	std::vector<std::string> currencies; // For summary/currency groups
};

class IBossProvider {
public:
	virtual ~IBossProvider() = default;
	virtual std::vector<BossGroup> GetBossGroups() const = 0;
	virtual std::string GetProofIdentifier(Boss boss, BossCategory category) const = 0;
	virtual std::string GetProofIdentifier(const std::string& currency) const = 0;
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