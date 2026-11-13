#include "kpme_provider.h"
#include <algorithm>
#include <optional>

namespace {
	constexpr const char* currencies[] = {"Legendary Insight", "Unstable Fractal Essence", "Boneskinner Ritual Vial"};

	struct KpmeTab {
		const char* name;
		const char* tableName;
		const char* category;
		std::optional<Content> content; // nullopt matches every content type
		BossType type;
		ProofKind kind;
	};

	constexpr KpmeTab tabs[] = {
			{"Raid Tokens",       "kpmeRaidsTable",     "Raid",   std::nullopt,    BossType::NORMAL, ProofKind::TOKEN },
			{"Raid CM Coffers",   "kpmeRaidCMsTable",   "Raid",   Content::RAID,   BossType::CM,     ProofKind::COFFER},
			{"Strike Coffers",    "kpmeStrikesTable",   "Strike", Content::STRIKE, BossType::NORMAL, ProofKind::COFFER},
			{"Strike CM Coffers", "kpmeStrikeCMsTable", "Strike", Content::STRIKE, BossType::CM,     ProofKind::COFFER},
	};

	const char* ItemName(const BossInfo& boss, BossType type, ProofKind kind) {
		if (kind == ProofKind::TOKEN) return boss.kpmeToken;
		return type == BossType::CM ? boss.kpmeCmCoffer : boss.kpmeCoffer;
	}

	std::vector<BossEntry> EntriesFor(const KpmeTab& tab) {
		std::vector<BossEntry> entries;
		for (const auto& boss : AllBosses()) {
			if (tab.content && boss.content != *tab.content) continue;
			if (!ItemName(boss, tab.type, tab.kind)) continue;
			entries.push_back({&boss, tab.type, tab.kind, ItemName(boss, tab.type, tab.kind)});
		}
		return entries;
	}
} // namespace

KpmeProvider::KpmeProvider() {
	bossGroups_.push_back({"Summary", "kpmeSummaryTable", {}, {std::begin(currencies), std::end(currencies)}});
	for (const auto& tab : tabs) {
		bossGroups_.push_back({tab.name, tab.tableName, EntriesFor(tab), {}});
	}
}

std::vector<ProofOption> KpmeProvider::GetAvailableProofs() const {
	std::vector<ProofOption> proofs;
	for (const auto* currency : currencies) {
		proofs.push_back({currency, currency, "Summary", "Normal"});
	}
	for (const auto& tab : tabs) {
		for (const auto& entry : EntriesFor(tab)) {
			proofs.push_back({entry.proofId, GetBossName(*entry.boss, entry.type, entry.kind), tab.category, entry.type == BossType::CM ? "CM" : "Normal"});
		}
	}
	return proofs;
}

BossGroup KpmeProvider::CreateCustomBossGroup(const CustomTab& tab) const {
	if (tab.id.empty() || tab.displayName.empty()) {
		return BossGroup {};
	}

	std::vector<BossEntry> bosses;
	std::vector<std::string> selectedCurrencies;
	for (const auto& proof : tab.proofs) {
		if (std::find(std::begin(currencies), std::end(currencies), proof.proofId) != std::end(currencies)) {
			selectedCurrencies.push_back(proof.proofId);
			continue;
		}
		for (const auto& kpmeTab : tabs) {
			auto entries = EntriesFor(kpmeTab);
			auto it = std::find_if(entries.begin(), entries.end(), [&](const BossEntry& entry) { return entry.proofId == proof.proofId; });
			if (it != entries.end()) {
				bosses.push_back(*it);
				break;
			}
		}
	}

	if (bosses.empty() && selectedCurrencies.empty()) {
		return BossGroup {};
	}
	return BossGroup {tab.displayName, tab.id + "Table", bosses, selectedCurrencies};
}
