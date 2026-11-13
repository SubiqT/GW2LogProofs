#include "wingman_provider.h"

namespace {
	std::shared_ptr<const PlayerProofs> Convert(const Wingman::WingmanResponse& response) {
		auto proofs = std::make_shared<PlayerProofs>();
		proofs->profileId = response.account;
		proofs->profileUrl = "https://gw2wingman.nevermindcreations.de/kp/" + response.account;
		proofs->own = {response.kp.begin(), response.kp.end()};
		proofs->combined = proofs->own;
		return proofs;
	}
} // namespace

void WingmanProvider::LoadPlayerDataAsync(const std::string& account, PlayerProofsCallback callback) {
	client_.GetKpAsync(account, [callback](const Wingman::WingmanResponse& response) {
		callback(Convert(response));
	});
}
