#include "wingman_client.h"
#include "../../core/shared.h"
#include "../../nlohmann/json.hpp"
#include "../../utils/httpclient.h"
#include <format>


using json = nlohmann::json;

namespace Wingman {
	void from_json(const json& j, WingmanResponse& r) {
		try {
			if (j.contains("account")) {
				if (j.at("account").is_string()) {
					j.at("account").get_to(r.account);
				}
			}

			if (j.contains("kp")) {
				if (j.at("kp").is_object()) {
					for (const auto& item : j.at("kp").items()) {
						if (item.value().is_object()) {
							if (item.value().contains("total")) {
								if (item.value().at("total").is_number_integer()) {
									r.kp[item.key()] = item.value().at("total");
								}
							}
						}
					}
				}
			}
		} catch (const json::parse_error& ex) {
			APIDefs->Log(ELogLevel_WARNING, ADDON_NAME, std::format("Failed to parse Wingman JSON response: {}", ex.what()).c_str());
		}
	}

	void WingmanClient::GetKpAsync(const std::string& account, std::function<void(const WingmanResponse&)> callback) {
		std::string url = std::format("https://gw2wingman.nevermindcreations.de/api/kp?account={}", account);
		const char* cUrl = url.c_str();
		APIDefs->Log(ELogLevel_DEBUG, ADDON_NAME, std::format("Requesting Wingman data (async): {}", cUrl).c_str());
		std::wstring wUrl(cUrl, cUrl + strlen(cUrl));

		HTTPClient::GetRequestAsync(wUrl, [account, callback](const std::string& response) {
			if (response.empty()) {
				APIDefs->Log(ELogLevel_WARNING, ADDON_NAME, std::format("Empty response from Wingman API for account: {}", account).c_str());
				callback(WingmanResponse {});
				return;
			}
			try {
				json j = json::parse(response);
				callback(j.template get<WingmanResponse>());
			} catch (const json::parse_error& e) {
				APIDefs->Log(ELogLevel_WARNING, ADDON_NAME, std::format("Failed to parse Wingman response for {}: {}", account, e.what()).c_str());
				callback(WingmanResponse {});
			}
		});
	}
} // namespace Wingman