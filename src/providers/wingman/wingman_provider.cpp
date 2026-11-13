#include "wingman_provider.h"

namespace {
	PlayerProofData ConvertWingmanResponse(const Wingman::WingmanResponse& response) {
		PlayerProofData data;
		data.accountName = response.account;
		data.profileId = response.account;
		data.profileUrl = "https://gw2wingman.nevermindcreations.de/kp/" + response.account;
		for (const auto& [id, amount] : response.kp) {
			data.proofs[id] = ProofData {id, amount};
		}
		return data;
	}
} // namespace

void WingmanProvider::LoadPlayerDataAsync(const std::string& account, std::function<void(const PlayerProofData&)> callback) {
	client_.GetKpAsync(account, [callback](const Wingman::WingmanResponse& response) {
		callback(ConvertWingmanResponse(response));
	});
}
