#include "wingman_boss_provider.h"
#include "../../core/bosses.h"
#include <charconv>
#include <cstdlib>

namespace {
	struct WingmanTab {
		const char* name;
		const char* tableName;
		const char* category;
		Content content;
		BossType type;
	};

	constexpr WingmanTab tabs[] = {
			{"Raids",       "normalRaidsTable",   "Raid",    Content::RAID,    BossType::NORMAL},
			{"Raid CMs",    "cmRaidsTable",       "Raid",    Content::RAID,    BossType::CM    },
			{"Raid LMs",    "lmRaidsTable",       "Raid",    Content::RAID,    BossType::LCM   },
			{"Fractal CMs", "cmFractalsTable",    "Fractal", Content::FRACTAL, BossType::CM    },
			{"Strikes",     "normalStrikesTable", "Strike",  Content::STRIKE,  BossType::NORMAL},
			{"Strike CMs",  "cmStrikesTable",     "Strike",  Content::STRIKE,  BossType::CM    },
			{"Strike LMs",  "lmStrikesTable",     "Strike",  Content::STRIKE,  BossType::LCM   },
	};

	const char* BossTypeName(BossType type) {
		switch (type) {
			case BossType::CM:
				return "CM";
			case BossType::LCM:
				return "LCM";
			default:
				return "Normal";
		}
	}

	// Saved custom tabs store CMs as the negated boss id and everything else as the plain id.
	std::string CustomTabProofId(const BossInfo& boss, BossType type) {
		return std::to_string(type == BossType::CM ? -boss.id : boss.id);
	}
} // namespace

std::vector<BossGroup> WingmanBossProvider::GetBossGroups() const {
	std::vector<BossGroup> groups;
	for (const auto& tab : tabs) {
		BossGroup group {tab.name, tab.tableName, {}, {}};
		for (const auto& boss : AllBosses()) {
			if (boss.content == tab.content && boss.HasWingmanMode(tab.type)) {
				group.bosses.push_back({&boss, tab.type});
			}
		}
		groups.push_back(std::move(group));
	}
	return groups;
}

std::string WingmanBossProvider::GetProofIdentifier(const BossEntry& entry) const {
	switch (entry.type) {
		case BossType::CM:
			return "-" + std::to_string(entry.boss->id);
		case BossType::LCM:
			return "--" + std::to_string(entry.boss->id);
		default:
			return std::to_string(entry.boss->id);
	}
}

std::string WingmanBossProvider::GetProofIdentifier(const std::string& currency) const {
	return currency;
}

std::vector<ProofOption> WingmanBossProvider::GetAvailableProofs() const {
	std::vector<ProofOption> proofs;
	for (const auto& tab : tabs) {
		for (const auto& boss : AllBosses()) {
			if (boss.content == tab.content && boss.HasWingmanMode(tab.type)) {
				proofs.push_back({CustomTabProofId(boss, tab.type), GetBossName(boss, tab.type), tab.category, BossTypeName(tab.type)});
			}
		}
	}
	return proofs;
}

BossGroup WingmanBossProvider::CreateCustomBossGroup(const CustomTab& tab) const {
	std::vector<BossEntry> bosses;
	for (const auto& proof : tab.proofs) {
		int id = 0;
		const auto* end = proof.proofId.data() + proof.proofId.size();
		if (std::from_chars(proof.proofId.data(), end, id).ptr != end) continue;
		const BossInfo* boss = FindBoss(std::abs(id));
		if (!boss) continue;
		bosses.push_back({boss, BossTypeFromString(proof.bossType)});
	}
	return BossGroup {tab.displayName, tab.id + "Table", bosses, {}};
}
