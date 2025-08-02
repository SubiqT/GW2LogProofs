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

	WingmanResponse WingmanClient::GetKp(const std::string& account) {
		std::string url = std::format("https://gw2wingman.nevermindcreations.de/api/kp?account={}", account);
		const char* cUrl = url.c_str();
		APIDefs->Log(ELogLevel_DEBUG, ADDON_NAME, std::format("Requesting Wingman data: {}", cUrl).c_str());
		std::wstring wUrl(cUrl, cUrl + strlen(cUrl));
		std::string response = HTTPClient::GetRequest(wUrl.c_str());
		if (response.empty()) {
			APIDefs->Log(ELogLevel_WARNING, ADDON_NAME, std::format("Empty response from Wingman API for account: {}", account).c_str());
			return WingmanResponse {};
		}
		try {
			json j = json::parse(response);
			return j.template get<WingmanResponse>();
		} catch (const json::parse_error& e) {
			APIDefs->Log(ELogLevel_WARNING, ADDON_NAME, std::format("Failed to parse Wingman response for {}: {}", account, e.what()).c_str());
			return WingmanResponse {};
		}
	}
} // namespace Wingman