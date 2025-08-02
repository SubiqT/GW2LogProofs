#include "kpme_provider.h"
#include "../../core/settings.h"
#include "../../core/shared.h"
#include <format>

PlayerProofData KpmeProvider::LoadPlayerData(const std::string& account) {
	Kpme::KpmeResponse response = client_.GetKp(account);
	return ConvertKpmeResponse(response);
}

void KpmeProvider::LoadPlayerDataAsync(const std::string& account, std::function<void(const PlayerProofData&)> callback) {
	client_.GetKpAsync(account, [this, callback](const Kpme::KpmeResponse& response) {
		PlayerProofData data = ConvertKpmeResponse(response);
		callback(data);
	});
}

std::vector<std::string> KpmeProvider::GetSupportedProofTypes() const {
	return {"KILL_PROOF", "TOKEN", "COFFER", "TITLE"};
}

PlayerProofData KpmeProvider::ConvertKpmeResponse(const Kpme::KpmeResponse& response) {
	PlayerProofData data;
	data.accountName = response.id;
	data.profileId = response.id;
	data.profileUrl = "https://killproof.me/proof/" + response.id;
	data.hasLinkedAccounts = !response.shared.killproofs.empty() || !response.shared.tokens.empty() || !response.shared.coffers.empty() || !response.shared.titles.empty();
	data.rawData = response; // Store raw response for dynamic computation

	// Populate proofs directly
	bool includeLinked = Settings::LinkedAccountsMode == COMBINE_LINKED;

	for (const auto& kp : response.self.killproofs) {
		ProofData proof;
		proof.identifier = kp.first;
		proof.amount = includeLinked && response.shared.killproofs.count(kp.first) ? response.shared.killproofs.at(kp.first) : kp.second;
		proof.type = ProofType::KILL_PROOF;
		proof.displayName = kp.first;
		proof.url = "";
		data.proofs[kp.first] = proof;
	}

	for (const auto& token : response.self.tokens) {
		ProofData proof;
		proof.identifier = token.first;
		proof.amount = includeLinked && response.shared.tokens.count(token.first) ? response.shared.tokens.at(token.first) : token.second;
		proof.type = ProofType::TOKEN;
		proof.displayName = token.first;
		proof.url = "";
		data.proofs[token.first] = proof;
	}

	for (const auto& coffer : response.self.coffers) {
		ProofData proof;
		proof.identifier = coffer.first;
		proof.amount = includeLinked && response.shared.coffers.count(coffer.first) ? response.shared.coffers.at(coffer.first) : coffer.second;
		proof.type = ProofType::COFFER;
		proof.displayName = coffer.first;
		proof.url = "";
		data.proofs[coffer.first] = proof;
	}

	return data;
}

PlayerProofData KpmeProvider::ComputeProofsFromRawData(const PlayerProofData& rawData, bool includeLinkedAccounts) const {
	PlayerProofData data = rawData;
	data.proofs.clear();
	data.linkedAccounts.clear();

	try {
		const auto& response = std::any_cast<const Kpme::KpmeResponse&>(rawData.rawData);

		for (const auto& kp : response.self.killproofs) {
			ProofData proof;
			proof.identifier = kp.first;
			proof.amount = includeLinkedAccounts && response.shared.killproofs.count(kp.first) ? response.shared.killproofs.at(kp.first) : kp.second;
			proof.type = ProofType::KILL_PROOF;
			proof.displayName = kp.first;
			proof.url = "";
			data.proofs[kp.first] = proof;
		}

		if (includeLinkedAccounts) {
			for (const auto& kp : response.shared.killproofs) {
				if (!response.self.killproofs.count(kp.first)) {
					ProofData proof;
					proof.identifier = kp.first;
					proof.amount = kp.second;
					proof.type = ProofType::KILL_PROOF;
					proof.displayName = kp.first;
					proof.url = "";
					data.proofs[kp.first] = proof;
				}
			}
		}

		for (const auto& token : response.self.tokens) {
			ProofData proof;
			proof.identifier = token.first;
			proof.amount = includeLinkedAccounts && response.shared.tokens.count(token.first) ? response.shared.tokens.at(token.first) : token.second;
			proof.type = ProofType::TOKEN;
			proof.displayName = token.first;
			proof.url = "";
			data.proofs[token.first] = proof;
		}

		for (const auto& coffer : response.self.coffers) {
			ProofData proof;
			proof.identifier = coffer.first;
			proof.amount = includeLinkedAccounts && response.shared.coffers.count(coffer.first) ? response.shared.coffers.at(coffer.first) : coffer.second;
			proof.type = ProofType::COFFER;
			proof.displayName = coffer.first;
			proof.url = "";
			data.proofs[coffer.first] = proof;
		}

		// Populate individual linked accounts when showing separately
		if (Settings::LinkedAccountsMode == SPLIT_LINKED && !response.linked_accounts.empty()) {
			for (const auto& linkedAccount : response.linked_accounts) {
				LinkedAccountData linkedData;
				linkedData.accountName = linkedAccount.name;

				// Add killproofs
				for (const auto& kp : linkedAccount.data.killproofs) {
					ProofData proof;
					proof.identifier = kp.first;
					proof.amount = kp.second;
					proof.type = ProofType::KILL_PROOF;
					proof.displayName = kp.first;
					linkedData.proofs[kp.first] = proof;
				}

				// Add tokens
				for (const auto& token : linkedAccount.data.tokens) {
					ProofData proof;
					proof.identifier = token.first;
					proof.amount = token.second;
					proof.type = ProofType::TOKEN;
					proof.displayName = token.first;
					linkedData.proofs[token.first] = proof;
				}

				// Add coffers
				for (const auto& coffer : linkedAccount.data.coffers) {
					ProofData proof;
					proof.identifier = coffer.first;
					proof.amount = coffer.second;
					proof.type = ProofType::COFFER;
					proof.displayName = coffer.first;
					linkedData.proofs[coffer.first] = proof;
				}

				data.linkedAccounts.push_back(linkedData);
			}
		}
	} catch (const std::bad_any_cast& e) {
		APIDefs->Log(ELogLevel_WARNING, ADDON_NAME, std::format("Failed to cast raw data for {}: {}", rawData.accountName, e.what()).c_str());
	}

	return data;
}