#include "wingman_client.h"
#include "../../utils/httpclient.h"

namespace Wingman {
	std::string KillProofUrl(const std::string& account) {
		return "https://gw2wingman.nevermindcreations.de/api/kp?account=" + HTTPClient::EncodeUrlComponent(account);
	}

	WingmanResponse ParseResponse(const nlohmann::json& j) {
		WingmanResponse r;
		if (auto account = j.find("account"); account != j.end() && account->is_string()) {
			r.account = account->get<std::string>();
		}
		auto kp = j.find("kp");
		if (kp == j.end() || !kp->is_object()) return r;
		for (const auto& [id, entry] : kp->items()) {
			if (!entry.is_object()) continue;
			auto total = entry.find("total");
			if (total != entry.end() && total->is_number_integer()) {
				r.kp[id] = total->get<int>();
			}
		}
		return r;
	}
} // namespace Wingman
