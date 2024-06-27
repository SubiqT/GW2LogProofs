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
            if (!j.at("kp").is_null()) {
                for (const auto& outer : j.at("kp").items()) {
                    for (const auto& inner : outer.value().items()) {
                        if (inner.key() == "total") {
                            r.kp.insert({ outer.key(), inner.value()});  // { boss_id, total_kills }
                        }
                    }
                }
            }
        }
        catch (...) {
            r.kp = {};
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
            j.at("kpid").get_to(r.id);
        }
        catch (...) { r.id = ""; };

        // self
        try {
            for (auto& killproof : j.at("killproofs")) {
                r.self.killproofs.insert({ killproof.at("name"), killproof.at("amount") });
            }
        }
        catch (...) { r.self.killproofs = {}; }

        try {
            for (auto& token : j.at("tokens")) {
                r.self.tokens.insert({ token.at("name"), token.at("amount") });
            }
        }
        catch (...) { r.self.tokens = {}; };

        try {
            for (auto& coffer : j.at("coffers")) {
                r.self.coffers.insert({ coffer.at("name"), coffer.at("amount") });;
            }
        }
        catch (...) { r.self.coffers = {}; };

        try {
            for (auto& title : j.at("titles")) {
                r.self.titles.insert({ title.at("name"), title.at("amount") });
            }
        }
        catch (...) { r.self.titles = {}; }

        // shared
        try {
            if (!j.at("linked_totals").is_null()) {
                try {
                    for (auto& killproof : j.at("linked_totals").at("killproofs")) {
                        r.shared.killproofs.insert({ killproof.at("name"), killproof.at("amount") });
                    }
                }
                catch (...) { r.shared.killproofs = {}; };

                try {
                    for (auto& token : j.at("linked_totals").at("tokens")) {
                        r.shared.tokens.insert({ token.at("name"), token.at("amount") });
                    }
                }
                catch (...) { r.shared.tokens = {}; };

                try {
                    for (auto& coffer : j.at("linked_totals").at("coffers")) {
                        r.shared.coffers.insert({ coffer.at("name"), coffer.at("amount") });;
                    }
                }
                catch (...) { r.shared.coffers = {}; };

                try {
                    for (auto& title : j.at("linked_totals").at("titles")) {
                        r.shared.titles.insert({ title.at("name"), title.at("amount") });
                    }
                }
                catch (...) { r.shared.titles = {}; };
            }
        }
        catch (...) {};
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

