#include "nlohmann/json.hpp"

using json = nlohmann::json;

#include <string>
#include <map>

#include "kp_loader.h"
#include "httpclient.hpp"
#include "shared.h"

namespace Wingman {

    void from_json(const json& j, WingmanResponse& r) {
        j.at("account").get_to(r.account);
        j.at("kp").get_to<std::map<std::string, std::map<std::string, int>>>(r.kp);
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
            // kpid
            j.at("kpid").get_to(r.id);

            // kp
            for (auto& killproof : j.at("killproofs")) {
                r.self.killproofs.insert({ killproof.at("name"), killproof.at("amount") });
            }
            for (auto& token : j.at("tokens")) {
                r.self.tokens.insert({ token.at("name"), token.at("amount") });
            }
            for (auto& coffer : j.at("coffers")) {
                r.self.coffers.insert({ coffer.at("name"), coffer.at("amount") });;
            }
            for (auto& title : j.at("titles")) {
                r.self.titles.insert({ title.at("name"), title.at("amount") });
            }
        }
        catch (const std::exception& e) {
            APIDefs->Log(ELogLevel_DEBUG, ADDON_NAME, "Failed to parse kpme self.");
        }

        // kpShared
        try {
            if (!j.at("linked_totals").is_null()) {
                for (auto& killproof : j.at("linked_totals").at("killproofs")) {
                    r.shared.killproofs.insert({ killproof.at("name"), killproof.at("amount") });
                }
                for (auto& token : j.at("linked_totals").at("tokens")) {
                    r.shared.tokens.insert({ token.at("name"), token.at("amount") });
                }
                for (auto& coffer : j.at("linked_totals").at("coffers")) {
                    r.shared.coffers.insert({ coffer.at("name"), coffer.at("amount") });;
                }
                for (auto& title : j.at("linked_totals").at("titles")) {
                    r.shared.titles.insert({ title.at("name"), title.at("amount") });
                }
            } 
        }
        catch (const std::exception& e) {
            APIDefs->Log(ELogLevel_DEBUG, ADDON_NAME, "Failed to parse kpme shared.");
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

