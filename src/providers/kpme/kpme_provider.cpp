#include "kpme_provider.h"
#include "../../core/settings.h"

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

	return data;
}

PlayerProofData KpmeProvider::ComputeProofsFromRawData(const PlayerProofData& rawData, bool includeLinkedAccounts) const {
	PlayerProofData data = rawData;
	data.proofs.clear();

	try {
		const auto& response = std::any_cast<const Kpme::KpmeResponse&>(rawData.rawData);

		for (const auto& kp : response.self.killproofs) {
			ProofData proof;
			proof.identifier = kp.first;
			proof.amount = kp.second;
			if (includeLinkedAccounts && response.shared.killproofs.count(kp.first)) {
				proof.amount += response.shared.killproofs.at(kp.first);
			}
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
			proof.amount = token.second;
			if (includeLinkedAccounts && response.shared.tokens.count(token.first)) {
				proof.amount += response.shared.tokens.at(token.first);
			}
			proof.type = ProofType::TOKEN;
			proof.displayName = token.first;
			proof.url = "";
			data.proofs[token.first] = proof;
		}

		for (const auto& coffer : response.self.coffers) {
			ProofData proof;
			proof.identifier = coffer.first;
			proof.amount = coffer.second;
			if (includeLinkedAccounts && response.shared.coffers.count(coffer.first)) {
				proof.amount += response.shared.coffers.at(coffer.first);
			}
			proof.type = ProofType::COFFER;
			proof.displayName = coffer.first;
			proof.url = "";
			data.proofs[coffer.first] = proof;
		}
	} catch (const std::bad_any_cast&) {
		// Fallback if raw data is not available
	}

	return data;
}