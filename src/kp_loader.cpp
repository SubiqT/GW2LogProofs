#include "nlohmann/json.hpp"

using json = nlohmann::json;

#include <string>
#include <map>

#include "kp_loader.h"
#include "httpclient.hpp"
#include "shared.h"

namespace Wingman {

    void from_json(const json& j, WingmanResponse& r) {
        try {
            if (j.contains("kp")) {
                if (j.at("kp").is_object()) {
                    for (const auto& item : j.at("kp").items()) {
                        if (item.value().is_object()) {
                            if (item.value().contains("total")) {
                                if (item.value().at("total").is_number_integer()) {
                                    r.kp[item.key()] = item.value().at("total");  // { boss_id, total_kills }
                                }
                            }
                        }
                    }
                }
            }
        }
        catch (json::parse_error& ex) {
            APIDefs->Log(ELogLevel_WARNING, ADDON_NAME, std::format("failed to parse wingman response. \nexception details: {}", ex.what()).c_str());
        }
    }

    WingmanResponse GetKp(std::string account) {
        std::string url = std::format("https://gw2wingman.nevermindcreations.de/api/kp?account={}", account);
        const char* cUrl = url.c_str();
        APIDefs->Log(ELogLevel_DEBUG, ADDON_NAME, cUrl);
        std::wstring wUrl(cUrl, cUrl + strlen(cUrl));
        std::string response = HTTPClient::GetRequest(wUrl.c_str());
        json j = json::parse(response);
        return j.template get<WingmanResponse>();
    }
}

namespace Kpme {

    void from_json(const json& j, KpmeResponse& r) {
        try {
            if (j.contains("kpid")) {
                if (j.at("kpid").is_string()) {
                    j.at("kpid").get_to(r.id);
                }
            }
            
            if (j.contains("killproofs")) {
                if (j.at("killproofs").is_array()) {
                    for (const auto& item : j.at("killproofs")) {
                        if (item.is_object()) {
                            if (item.contains("name") && item.contains("amount")) {
                                if (item.at("name").is_string() && item.at("amount").is_number_integer()) {
                                    r.self.killproofs[item.at("name")] = item.at("amount");
                                }
                            }
                        }
                    }
                }
            }

            if (j.contains("tokens")) {
                if (j.at("tokens").is_array()) {
                    for (const auto& item : j.at("tokens")) {
                        if (item.is_object()) {
                            if (item.contains("name") && item.contains("amount")) {
                                if (item.at("name").is_string() && item.at("amount").is_number_integer()) {
                                    r.self.tokens[item.at("name")] = item.at("amount");
                                }
                            }
                        }
                    }
                }
            }

            if (j.contains("coffers")) {
                if (j.at("coffers").is_array()) {
                    for (const auto& item : j.at("coffers")) {
                        if (item.is_object()) {
                            if (item.contains("name") && item.contains("amount")) {
                                if (item.at("name").is_string() && item.at("amount").is_number_integer()) {
                                    r.self.coffers[item.at("name")] = item.at("amount");
                                }
                            }
                        }
                    }
                }
            }
            
            if (j.contains("titles")) {
                if (j.at("titles").is_array()) {
                    for (const auto& item : j.at("titles")) {
                        if (item.is_object()) {
                            if (item.contains("name") && item.contains("mode")) {
                                if (item.at("name").is_string() && item.at("mode").is_number_integer()) {
                                    r.self.titles[item.at("name")] = item.at("mode");
                                }
                            }
                        }
                    }
                }
            }

            if (j.contains("linked_totals")) {
                if (j.at("linked_totals").is_object()) {

                    if (j.contains("killproofs")) {
                        if (j.at("killproofs").is_array()) {
                            for (const auto& item : j.at("killproofs")) {
                                if (item.is_object()) {
                                    if (item.contains("name") && item.contains("amount")) {
                                        if (item.at("name").is_string() && item.at("amount").is_number_integer()) {
                                            r.shared.killproofs[item.at("name")] = item.at("amount");
                                        }
                                    }
                                }
                            }
                        }
                    }

                    if (j.contains("tokens")) {
                        if (j.at("tokens").is_array()) {
                            for (const auto& item : j.at("tokens")) {
                                if (item.is_object()) {
                                    if (item.contains("name") && item.contains("amount")) {
                                        if (item.at("name").is_string() && item.at("amount").is_number_integer()) {
                                            r.shared.tokens[item.at("name")] = item.at("amount");
                                        }
                                    }
                                }
                            }
                        }
                    }

                    if (j.contains("coffers")) {
                        if (j.at("coffers").is_array()) {
                            for (const auto& item : j.at("coffers")) {
                                if (item.is_object()) {
                                    if (item.contains("name") && item.contains("amount")) {
                                        if (item.at("name").is_string() && item.at("amount").is_number_integer()) {
                                            r.shared.coffers[item.at("name")] = item.at("amount");
                                        }
                                    }
                                }
                            }
                        }
                    }

                    if (j.contains("titles")) {
                        if (j.at("titles").is_array()) {
                            for (const auto& item : j.at("titles")) {
                                if (item.is_object()) {
                                    if (item.contains("name") && item.contains("mode")) {
                                        if (item.at("name").is_string() && item.at("mode").is_number_integer()) {
                                            r.shared.titles[item.at("name")] = item.at("mode");
                                        }
                                    }
                                }
                            }
                        }
                    }

                }
            }

        }
        catch (json::parse_error& ex) {
            APIDefs->Log(ELogLevel_WARNING, ADDON_NAME, std::format("failed to parse kpme response. \nexception details: {}", ex.what()).c_str());
        }
    }

    KpmeResponse GetKp(std::string account) {
        std::string url = std::format("https://killproof.me/api/kp/{}?lang=en", account);
        const char* cUrl = url.c_str();
        APIDefs->Log(ELogLevel_DEBUG, ADDON_NAME, cUrl);
        std::wstring wUrl(cUrl, cUrl + strlen(cUrl));
        std::string response = HTTPClient::GetRequest(wUrl.c_str());
        json j = json::parse(response);
        return j.template get<KpmeResponse>();
    }
}

