#include "wingman_provider.h"
#include "../common/json_fetch.h"
#include "wingman_client.h"
#include "../../utils/httpclient.h"

namespace {
	std::shared_ptr<const PlayerProofs> Convert(const Wingman::WingmanResponse& response) {
		auto proofs = std::make_shared<PlayerProofs>();
		proofs->profileId = response.account;
		proofs->profileUrl = "https://gw2wingman.nevermindcreations.de/kp/" + HTTPClient::EncodeUrlComponent(response.account);
		for (const auto& [id, total] : response.kp) {
			proofs->own[id] = {total};
		}
		proofs->combined = proofs->own;
		return proofs;
	}
} // namespace

void WingmanProvider::FetchAsync(const std::string& account, FetchCallback callback) {
	FetchJson(Wingman::KillProofUrl(account), [](const nlohmann::json& j) { return Convert(Wingman::ParseResponse(j)); }, std::move(callback));
}
