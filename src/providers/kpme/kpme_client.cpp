#include "kpme_client.h"
#include "../../utils/httpclient.h"

using json = nlohmann::json;

namespace Kpme {
	namespace {
		// Sums repeated entries: the API can list the same item name more than once under different item ids.
		void ReadCounts(const json& parent, const char* key, std::map<std::string, int>& counts) {
			auto list = parent.find(key);
			if (list == parent.end() || !list->is_array()) return;
			for (const auto& item : *list) {
				if (!item.is_object()) continue;
				auto name = item.find("name");
				auto amount = item.find("amount");
				if (name == item.end() || amount == item.end() || !name->is_string() || !amount->is_number_integer()) continue;
				counts[name->get<std::string>()] += amount->get<int>();
			}
		}

		KpSummary ReadSummary(const json& j) {
			KpSummary summary;
			ReadCounts(j, "killproofs", summary.killproofs);
			ReadCounts(j, "tokens", summary.tokens);
			ReadCounts(j, "coffers", summary.coffers);
			return summary;
		}
	} // namespace

	std::string KillProofUrl(const std::string& account) {
		return "https://killproof.me/api/kp/" + HTTPClient::EncodeUrlComponent(account) + "?lang=en";
	}

	KpmeResponse ParseResponse(const json& j) {
		KpmeResponse r;
		if (auto id = j.find("kpid"); id != j.end() && id->is_string()) {
			r.id = id->get<std::string>();
		}
		r.self = ReadSummary(j);
		if (auto totals = j.find("linked_totals"); totals != j.end() && totals->is_object()) {
			r.shared = ReadSummary(*totals);
		}
		if (auto linked = j.find("linked"); linked != j.end() && linked->is_array()) {
			for (const auto& account : *linked) {
				if (!account.is_object()) continue;
				auto name = account.find("account_name");
				if (name == account.end() || !name->is_string()) continue;
				r.linked_accounts.push_back({name->get<std::string>(), ReadSummary(account)});
			}
		}
		return r;
	}
} // namespace Kpme
