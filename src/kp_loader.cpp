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

    WingmanResponse GetKillProofs(std::string account) {
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
        // kpid
        j.at("kpid").get_to(r.kpId);

        // linked
        for (auto& account : j.at("linked")) {
            AccountKp accKp = {};
            account.at("account_name").get_to(accKp.account);
            for (auto& killproof : j.at("killproofs")) {
                Kp kp = {};
                killproof.at("id").get_to(kp.id);
                killproof.at("name").get_to(kp.name);
                killproof.at("amount").get_to(kp.amount);
                accKp.killproofs.push_back(kp);
            }
            for (auto& token : j.at("tokens")) {
                Kp kp = {};
                token.at("id").get_to(kp.id);
                token.at("name").get_to(kp.name);
                token.at("amount").get_to(kp.amount);
                accKp.tokens.push_back(kp);
            }
            for (auto& coffer : j.at("coffers")) {
                Kp kp = {};
                coffer.at("id").get_to(kp.id);
                coffer.at("name").get_to(kp.name);
                coffer.at("amount").get_to(kp.amount);
                accKp.coffers.push_back(kp);
            }
            for (auto& title : j.at("titles")) {
                Title ttl = {};
                title.at("id").get_to(ttl.id);
                title.at("name").get_to(ttl.name);
                title.at("mode").get_to(ttl.mode);
                accKp.titles.push_back(ttl);
            }
            r.linked.push_back(accKp);
        }

        // linkedTotals
        for (auto& killproof : j.at("killproofs")) {
            Kp kp = {};
            killproof.at("id").get_to(kp.id);
            killproof.at("name").get_to(kp.name);
            killproof.at("amount").get_to(kp.amount);
            r.linkedTotals.killproofs.push_back(kp);
        }
        for (auto& token : j.at("tokens")) {
            Kp kp = {};
            token.at("id").get_to(kp.id);
            token.at("name").get_to(kp.name);
            token.at("amount").get_to(kp.amount);
            r.linkedTotals.tokens.push_back(kp);
        }
        for (auto& coffer : j.at("coffers")) {
            Kp kp = {};
            coffer.at("id").get_to(kp.id);
            coffer.at("name").get_to(kp.name);
            coffer.at("amount").get_to(kp.amount);
            r.linkedTotals.coffers.push_back(kp);
        }
        for (auto& title : j.at("titles")) {
            Title ttl = {};
            title.at("id").get_to(ttl.id);
            title.at("name").get_to(ttl.name);
            title.at("mode").get_to(ttl.mode);
            r.linkedTotals.titles.push_back(ttl);
        }
    }

    KpmeResponse GetKillProofs(std::string account) {
        std::string url = std::format("https://killproof.me/api/kp/{}?lang=en", account);
        const char* cUrl = url.c_str();
        APIDefs->Log(ELogLevel_DEBUG, ADDON_NAME, cUrl);
        std::wstring wUrl(cUrl, cUrl + strlen(cUrl));
        std::string response = HTTPClient::GetRequest(wUrl.c_str());
        json j = json::parse(response);
        return j.template get<KpmeResponse>();
    }
}

