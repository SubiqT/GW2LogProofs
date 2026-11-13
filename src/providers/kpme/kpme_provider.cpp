#include "kpme_provider.h"
#include "../common/json_fetch.h"
#include "kpme_client.h"
#include "../../core/bosses.h"

namespace {
	constexpr int TOKENS_PER_RAID_COFFER = 3;

	void AddMissing(ProofCounts& counts, const std::map<std::string, int>& source) {
		for (const auto& [name, amount] : source) {
			counts.try_emplace(name, amount);
		}
	}

	// Counts as shown in the tables: Legendary Divination counts towards Legendary Insight and each
	// raid coffer counts as three of that boss's tokens.
	ProofCounts ToDisplayCounts(const Kpme::KpSummary& summary) {
		ProofCounts counts;
		AddMissing(counts, summary.killproofs);
		AddMissing(counts, summary.tokens);
		AddMissing(counts, summary.coffers);

		if (auto divination = summary.killproofs.find("Legendary Divination"); divination != summary.killproofs.end()) {
			counts["Legendary Insight"] += divination->second;
		}
		for (const auto& boss : AllBosses()) {
			if (boss.content != Content::RAID || !boss.kpmeToken || !boss.kpmeCoffer) continue;
			if (auto coffers = summary.coffers.find(boss.kpmeCoffer); coffers != summary.coffers.end()) {
				counts[boss.kpmeToken] += coffers->second * TOKENS_PER_RAID_COFFER;
			}
		}
		return counts;
	}

	Kpme::KpSummary Combine(const Kpme::KpSummary& own, const Kpme::KpSummary& linkedTotals) {
		auto overlay = [](std::map<std::string, int> base, const std::map<std::string, int>& totals) {
			for (const auto& [name, amount] : totals) base[name] = amount;
			return base;
		};
		return {overlay(own.killproofs, linkedTotals.killproofs), overlay(own.tokens, linkedTotals.tokens), overlay(own.coffers, linkedTotals.coffers)};
	}

	std::shared_ptr<const PlayerProofs> Convert(const Kpme::KpmeResponse& response) {
		auto proofs = std::make_shared<PlayerProofs>();
		proofs->profileId = response.id;
		proofs->profileUrl = "https://killproof.me/proof/" + response.id;
		proofs->own = ToDisplayCounts(response.self);
		proofs->combined = ToDisplayCounts(Combine(response.self, response.shared));
		for (const auto& linkedAccount : response.linked_accounts) {
			proofs->linkedAccounts.push_back({linkedAccount.name, ToDisplayCounts(linkedAccount.data)});
		}
		return proofs;
	}
} // namespace

void KpmeProvider::FetchAsync(const std::string& account, FetchCallback callback) {
	FetchJson(Kpme::KillProofUrl(account), [](const nlohmann::json& j) { return Convert(Kpme::ParseResponse(j)); }, std::move(callback));
}
